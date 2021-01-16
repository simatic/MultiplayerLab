//
// Created by jglrxavpok on 15/10/2020.
//
#include "Server/Interface.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <implot.h>
#include <Server/ServerClock.h>
#include <Server/DelayCreation.h>

Interface::Interface(ServerNetworkHandler& handler): serverNetwork(handler) {
    handler.registerListener(this);
}

Interface::~Interface() {
    ImGui::DestroyContext();
    ImPlot::DestroyContext();
}

template<typename Getter, typename Setter>
void DragFloat(const char* label, Getter get, Setter set, float min = 0.0f, float max = 1.0f) {
    float value = get();
    if(ImGui::SliderFloat(label, &value, min, max)) {
        set(value);
    }
}

static ImVec4 TITLE_COLOR = ImVec4(1,1,0,1);

void Interface::render(sf::RenderWindow& window, float clientWidth, float interfaceHeight, float startY) {
    if(!implotInit) {
        ImGui::SFML::Init(window);
        ImPlot::CreateContext();

        const ImVec4 colors[] = {
                ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // client 0
                ImVec4(0.0f, 1.0f, 0.0f, 1.0f), // client 1
                ImVec4(0.0f, 0.0f, 1.0f, 1.0f), // server
        };
        ImPlot::SetColormap(colors, 3);

        ImPlot::GetStyle().Colors[ImPlotCol_FrameBg] = ImVec4(0,0,0,0);

        ImGui::GetStyle().WindowRounding = 0.0f;
        ImGui::GetStyle().WindowBorderSize = 0.0f;
        implotInit = true;
    }

    ImGui::SFML::Update(window, deltaClock.restart());

    auto& clients = serverNetwork.getClients();

    if(clients.empty()) {
        ImGui::SetNextWindowPos(ImVec2{clientWidth, startY + interfaceHeight / 2.0f});
        if(ImGui::Begin("En attente de clients", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)) {
            ImGui::Text("Les fenêtres des clients arriveront ici quand ils se connecteront.");
        }
        ImGui::End();
    } else {
        auto flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
        ImGui::SetNextWindowPos(ImVec2{0, startY});
        ImGui::SetNextWindowSize(ImVec2{clientWidth*2, interfaceHeight});
        if(ImGui::Begin("Graphes", nullptr, flags)) {
            renderIncomingPackets(clientWidth, interfaceHeight);
            ImGui::SameLine();

            // make subwindows have the same width, and make it so they are ""correctly"" aligned with game views
            ImGui::Dummy(ImVec2(clientWidth*0.025, 0));
            ImGui::SameLine();
            renderOutcomingPackets(clientWidth, interfaceHeight);
        }
        ImGui::End();
    }

    ImGui::SFML::Render(window);
}

void Interface::renderIncomingGraph() {
    static bool pauseGraphScroll = false;
    static float zoomFactor = 0.015f;

    ImGui::Checkbox("Mettre en pause le scrolling", &pauseGraphScroll);

    float time = ServerClock::getInstance().asSeconds();

    // pause? => ImGuiCond_Once
    ImPlot::SetNextPlotLimitsX(time-5*zoomFactor, time, !pauseGraphScroll ? ImGuiCond_Always : ImGuiCond_Once);
    ImPlot::SetNextPlotLimitsY(-2, 2, ImGuiCond_Always);

    auto yFlags = ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels;
    // TODO: merge code with incoming?
    if(ImPlot::BeginPlot("Graphe des packets en entrée", "Temps", nullptr, ImVec2(-1,0), ImPlotFlags_None, ImPlotAxisFlags_None, yFlags)) {
        if(ImPlot::IsPlotHovered()) {
            zoomFactor -= ImGui::GetIO().MouseWheel/10.0f;

            zoomFactor = std::min(100.0f, std::max(0.01f, zoomFactor));
        }
        auto& client0Packets = clientEvents[serverNetwork.getClients()[0]->id];
        auto& client1Packets = clientEvents[serverNetwork.getClients()[1]->id];

        // TODO: window data to avoid FPS drops
        ImPlot::PlotScatterG("Packet envoyé par le client 1", [](void* ptr, int index) {
            auto* timestamps = static_cast<float*>(ptr);
            float timestamp = timestamps[index];
            return ImPlotPoint(timestamp, 1);
        }, client0Packets.receivedTimestamps.data(), client0Packets.receivedTimestamps.size());
        ImPlot::PlotScatterG("Packet envoyé par le client 2", [](void* ptr, int index) {
            auto* timestamps = static_cast<float*>(ptr);
            float timestamp = timestamps[index];
            return ImPlotPoint(timestamp, -1);
        }, client1Packets.receivedTimestamps.data(), client1Packets.receivedTimestamps.size());

        ImPlot::PlotScatterG("Packet reçu", [](void* ptr, int index) {
            auto* timestamps = static_cast<float*>(ptr);
            float timestamp = timestamps[index];
            return ImPlotPoint(timestamp, 0);
        }, afterDelayTimestamps.data(), afterDelayTimestamps.size());

        ImPlot::EndPlot();
    }
}

void Interface::renderOutgoingGraph() {
    static bool pauseGraphScroll = false;
    static float zoomFactor = 0.015f;

    ImGui::Checkbox("Mettre en pause le scrolling", &pauseGraphScroll);

    float time = ServerClock::getInstance().asSeconds();

    // pause? => ImGuiCond_Once
    ImPlot::SetNextPlotLimitsX(time-5*zoomFactor, time, !pauseGraphScroll ? ImGuiCond_Always : ImGuiCond_Once);
    ImPlot::SetNextPlotLimitsY(-2, 2, ImGuiCond_Always);

    auto yFlags = ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels;

    // TODO: merge code with incoming?
    if(ImPlot::BeginPlot("Graphe des packets en sortie", "Temps", nullptr, ImVec2(-1,0), ImPlotFlags_None, ImPlotAxisFlags_None, yFlags)) {
        if(ImPlot::IsPlotHovered()) {
            zoomFactor -= ImGui::GetIO().MouseWheel/10.0f;

            zoomFactor = std::min(100.0f, std::max(0.01f, zoomFactor));
        }
        auto& client0Packets = clientEvents[serverNetwork.getClients()[0]->id];
        auto& client1Packets = clientEvents[serverNetwork.getClients()[1]->id];

        // TODO: window data to avoid FPS drops
        ImPlot::PlotScatterG("Packets envoyés au client 1", [](void* ptr, int index) {
            auto* timestamps = static_cast<float*>(ptr);
            float timestamp = timestamps[index];
            return ImPlotPoint(timestamp, 1);
        }, client0Packets.sentTimestamps.data(), client0Packets.sentTimestamps.size());
        ImPlot::PlotScatterG("Packets envoyés au client 2", [](void* ptr, int index) {
            auto* timestamps = static_cast<float*>(ptr);
            float timestamp = timestamps[index];
            return ImPlotPoint(timestamp, -1);
        }, client1Packets.sentTimestamps.data(), client1Packets.sentTimestamps.size());

        ImPlot::PlotScatterG("Packet envoyé", [](void* ptr, int index) {
            auto* timestamps = static_cast<float*>(ptr);
            float timestamp = timestamps[index];
            return ImPlotPoint(timestamp, 0);
        }, sendingStartsTimestamps.data(), sendingStartsTimestamps.size());

        ImPlot::EndPlot();
    }
}

void Interface::renderIncomingPackets(float clientWidth, float height) {
    float width = clientWidth*0.97f;
    ImGui::BeginChild("Incoming packets", ImVec2(width, height*0.95f));

    renderIncomingGraph();

    auto& clients = serverNetwork.getClients();
    for(size_t i = 0; i < clients.size(); i++) {
        if(i != 0) {
            ImGui::SameLine();
        }
        const char* ids[] = {
                "Client in #0",
                "Client in #1",
        };
        ImGui::BeginChild(ids[i], ImVec2(width/2.0f, 0));
        ImGui::TextColored(TITLE_COLOR, "Client #%llu", i+1);

        auto& client = *clients[i];
        ImGui::SetNextItemWidth(width/4.0f);
        DragFloat("Pertes de paquet en entrée", [&]{return client.settings.getPercentageInComingPacketLost();}, [&](float v){client.settings.setPercentageInComingPacketLost(v);});

        ImGui::SetNextItemWidth(width/4.0f);
        DragFloat("Délai avant réception", [&]{return client.settings.getIncomingDelay();}, [&](float v){client.settings.setIncomingDelay(v);}, 0, 10.0f);
        ImGui::EndChild();
    }

    ImGui::EndChild();
}

void Interface::renderOutcomingPackets(float clientWidth, float height) {
    float width = clientWidth*0.97f;
    ImGui::BeginChild("Outcoming packets", ImVec2(width, height*0.95f));

    renderOutgoingGraph();

    auto& clients = serverNetwork.getClients();
    for(size_t i = 0; i < clients.size(); i++) {
        if(i != 0) {
            ImGui::SameLine();
        }
        const char* ids[] = {
                "Client out #0",
                "Client out #1",
        };
        ImGui::BeginChild(ids[i], ImVec2(width/2.0f, 0));
        ImGui::TextColored(TITLE_COLOR, "Client #%llu", i+1);

        auto& client = *clients[i];
        ImGui::SetNextItemWidth(width/4.0f);
        DragFloat("Pertes de paquet en sortie", [&]{return client.settings.getPercentageOutGoingPacketLost();}, [&](float v){client.settings.setPercentageOutGoingPacketLost(v);});

        ImGui::SetNextItemWidth(width/4.0f);
        DragFloat("Délai avant envoi", [&]{return client.settings.getOutgoingDelay();}, [&](float v){client.settings.setOutgoingDelay(v);}, 0, 10.0f);
        ImGui::EndChild();
    }

    ImGui::EndChild();
}

void Interface::onEvent(const UdpClient &client, NetworkEvent::Event event) {
    if(clientEvents.find(client.id) == clientEvents.end()) {
        clientEvents[client.id] = {};
    }
    if(clientPacketLifecycles.find(client.id) == clientPacketLifecycles.end()) {
        clientPacketLifecycles[client.id] = std::vector<PacketLifecycle>{};
    }
    auto& compilation = clientEvents[client.id];

    switch(event.type) {
        case NetworkEvent::PacketReceived: {
            compilation.receivedTimestamps.push_back(event.timestamp.asSeconds());
            compilation.receivedPacketIndices.push_back(event.id);
        } break;

        case NetworkEvent::PacketDelayed: {
            afterDelayTimestamps.push_back(event.timestamp.asSeconds());
            afterDelayPacketIndices.push_back(event.id);
        } break;

        case NetworkEvent::SendingPacket: {
            sendingStartsTimestamps.push_back(event.timestamp.asSeconds());
            sendingStartsPacketIndices.push_back(event.id);
        } break;

        case NetworkEvent::SentPacket: {
            compilation.sentTimestamps.push_back(event.timestamp.asSeconds());
            compilation.sentPacketIndices.push_back(event.id);
        } break;
    }

    // TODO: packet links
}

void Interface::linkPackets(const UdpClient &client, const NetworkEvent::Event &event,
                            const std::map<NetworkEvent::Type, Interface::CompiledEvents> &eventMap,
                            const NetworkEvent::Type typeToLinkTo) {
    auto receivedPacketEventsIt = eventMap.find(typeToLinkTo);
    if(receivedPacketEventsIt == eventMap.end()) { // no event of given type
        return;
    }
    // TODO: re-do
    /*
    auto& receivedPacketEvents = receivedPacketEventsIt->second;
    auto it = std::find(receivedPacketEvents.packetIndices.begin(), receivedPacketEvents.packetIndices.end(), event.id);
    if(it == receivedPacketEvents.packetIndices.end()) // no packet to link to
        return;
    int position = std::distance(receivedPacketEvents.packetIndices.begin(), it);
    float receivedTime = receivedPacketEvents.timestamps[position];
    float receivedValue = receivedPacketEvents.values[position];
    auto& lifecycles = clientPacketLifecycles[client.id];
    // edge from received to delayed event
    lifecycles.push_back(PacketLifecycle{event.id,
                            std::make_pair(sf::Vector2f(receivedTime, receivedValue),
                                           sf::Vector2f(event.timestamp.asSeconds(), static_cast<float>(event.type))
                                           )});*/
}
