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

void Interface::render(sf::RenderWindow& window, float clientWidth, float clientHeight, float startY) {
    if(!implotInit) {
        ImGui::SFML::Init(window);
        ImPlot::CreateContext();

        implotInit = true;
    }

    ImGui::SFML::Update(window, deltaClock.restart());

    auto& clients = serverNetwork.getClients();
    for (int i = 0; i < clients.size(); ++i) {
        auto& client = clients.at(i);
        ImGui::SetNextWindowPos(ImVec2{i*clientWidth, startY});
        ImGui::SetNextWindowSize(ImVec2{clientWidth, clientHeight});
        renderClientWindow("Client #" + std::to_string(i+1), *client);
    }

    if(clients.empty()) {
        ImGui::SetNextWindowPos(ImVec2{clientWidth, startY+clientHeight/2.0f});
        if(ImGui::Begin("En attente de clients", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)) {
            ImGui::Text("Les fenêtres des clients arriveront ici quand ils se connecteront.");
        }
        ImGui::End();
    }

    ImGui::SFML::Render(window);
}

void Interface::renderClientWindow(const std::string& title, UdpClient& client) {
    auto flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
    if(ImGui::Begin(("Contrôles "+title+" IP;Port: "+client.address.toString()+";"+std::to_string(client.port)).c_str(), nullptr, flags)) {
        renderGraph(client);

        ImGui::Separator();

        float width = ImGui::GetWindowWidth()*0.95f;
        float childWidth = width/2.0f;
        ImGui::BeginChild("PacketLoss", ImVec2(width/2.0f, 0), false);
        {
            ImGui::TextColored(TITLE_COLOR, "Pertes de packets (en pourcentage)");

            ImGui::SetNextItemWidth(childWidth/4.0f);
            DragFloat("Pertes client", [&]{return client.settings.getPercentageInComingPacketLost();}, [&](float v){client.settings.setPercentageInComingPacketLost(v);});

            ImGui::SetNextItemWidth(childWidth/4.0f);
            ImGui::SameLine();
            DragFloat("Pertes serveur", [&]{return client.settings.getPercentageOutGoingPacketLost();}, [&](float v){client.settings.setPercentageOutGoingPacketLost(v);});
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Delays", ImVec2(width/2.0f, 0), false);
        {
            ImGui::TextColored(TITLE_COLOR, "Délais (en secondes)");

            ImGui::SetNextItemWidth(childWidth/4.0f);
            DragFloat("Avant traitement", [&]{return client.settings.getIncomingDelay();}, [&](float v){client.settings.setIncomingDelay(v);}, 0, 10.0f);

            ImGui::SetNextItemWidth(childWidth/4.0f);
            ImGui::SameLine();
            DragFloat("Avant envoi", [&]{return client.settings.getOutgoingDelay();}, [&](float v){client.settings.setOutgoingDelay(v);}, 0, 10.0f);
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void Interface::renderGraph(const UdpClient &client) {
    bool& pauseGraphScroll = pauseGraphForClient[client.id];
    ImGui::Checkbox("Mettre en pause le scrolling", &pauseGraphScroll);

    float time = ServerClock::getInstance().asSeconds();

    // pause? => ImGuiCond_Once
    ImPlot::SetNextPlotLimitsX(time-10, time, !pauseGraphScroll ? ImGuiCond_Always : ImGuiCond_Once);
    ImPlot::SetNextPlotLimitsY(-1, NetworkEvent::Last, ImGuiCond_Once);

    if(ImPlot::BeginPlot("Graphes des packets")) {
        CompiledEventsMap& events = clientEvents[client.id];

        for(unsigned int typeID = 0; typeID < NetworkEvent::Last; typeID++) {
            auto type = static_cast<NetworkEvent::Type>(typeID);

            if(events.find(type) == events.end())
                continue;

            auto& timestamps = events[type].timestamps;
            auto& values = events[type].values;

            ImPlot::PlotScatter(NetworkEvent::name(type), timestamps.data(), values.data(), values.size());
        }

        // render connections between packets
        std::vector<PacketLifecycle>& packetLives = clientPacketLifecycles[client.id];
        for(auto& lifecycle : packetLives) {
            float times[2] = { lifecycle.edges.first.x, lifecycle.edges.second.x };
            float values[2] = { lifecycle.edges.first.y, lifecycle.edges.second.y };
            ImPlot::PlotLine("", times, values, 2);
        }

        // render tooltip with packet information
        if(ImPlot::IsPlotHovered()) {
            ImPlotPoint mouse = ImPlot::GetPlotMousePos();

            const std::pair<const PacketInfo, const PacketLifecycle*> closest = getClosest(events, packetLives, mouse.x, mouse.y);

            const PacketInfo closestPacketInfo = closest.first;
            const PacketLifecycle* closestLifecycle = closest.second;

            // if found a packet close to the mouse, render the tooltip
            if(closestPacketInfo.sequenceIndex != 0) {
                ImGui::BeginTooltip();
                ImGui::TextColored(TITLE_COLOR, "Informations du packet");

                const char* source;
                auto sequenceIndex = closestPacketInfo.sequenceIndex;
                if(sequenceIndex < 0) { // client-generated
                    source = "Client";
                } else {
                    source = "Serveur";
                }
                ImGui::Text("N° de séquence: %lld (initié par le %s)", abs(sequenceIndex), source);
                ImGui::Text("Type de packet: %s", NetworkEvent::name(closestPacketInfo.packetType));

                if(closestLifecycle != nullptr) {
                    const auto& firstPoint = closestLifecycle->edges.first;
                    const auto& secondPoint = closestLifecycle->edges.second;
                    long delay = static_cast<long>((secondPoint.x-firstPoint.x)*1000);
                    ImGui::Text("Délai: %ldms", delay);
                } else {
                    ImGui::Text("Packet orphelin");
                }
                ImGui::EndTooltip();
            }
        }

        ImPlot::EndPlot();
    }
}

void Interface::onEvent(const UdpClient &client, NetworkEvent::Event event) {
    if(clientEvents.find(client.id) == clientEvents.end()) {
        clientEvents[client.id] = CompiledEventsMap{};
    }
    if(clientPacketLifecycles.find(client.id) == clientPacketLifecycles.end()) {
        clientPacketLifecycles[client.id] = std::vector<PacketLifecycle>{};
    }
    auto& eventMap = clientEvents[client.id];
    if(eventMap.find(event.type) == eventMap.end()) {
        eventMap[event.type] = CompiledEvents();
    }
    auto& compilation = eventMap[event.type];
    compilation.timestamps.push_back(event.timestamp.asSeconds());
    compilation.values.push_back(event.type);
    compilation.packetIndices.push_back(event.id);

    // build connections between events
    if(event.type == NetworkEvent::Type::PacketDelayed
   || event.type == NetworkEvent::Type::SendingPacket
   || event.type == NetworkEvent::Type::SentPacket) {
        linkPackets(client, event, eventMap, static_cast<NetworkEvent::Type>(event.type-1));
    }
}

void Interface::linkPackets(const UdpClient &client, const NetworkEvent::Event &event,
                            const std::map<NetworkEvent::Type, Interface::CompiledEvents> &eventMap,
                            const NetworkEvent::Type typeToLinkTo) {
    auto receivedPacketEventsIt = eventMap.find(typeToLinkTo);
    if(receivedPacketEventsIt == eventMap.end()) { // no event of given type
        return;
    }
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
                                           )});
}

static float sqDist(sf::Vector2f vec, float x, float y) {
    float dx = vec.x - x;
    float dy = vec.y - y;
    return dx*dx+dy*dy;
}

std::pair<const Interface::PacketInfo, const Interface::PacketLifecycle*> Interface::getClosest(const CompiledEventsMap& packets, const std::vector<PacketLifecycle> &lifecycles, float x, float y) {
    const PacketLifecycle* closestLifecycle = nullptr;
    PacketInfo packetInfo{};
    // TODO: optimize via binary search (via order of 'first.x' and 'second.x') ?
    float smallestDistanceLifecycle = INFINITY;
    float smallestDistancePacket = INFINITY;
    for(const PacketLifecycle& lifecycle : lifecycles) {
        float distanceSentTime = sqDist(lifecycle.edges.first, x, y);
        float distanceReceivedTime = sqDist(lifecycle.edges.second, x, y);

        if(smallestDistanceLifecycle > distanceSentTime) {
            smallestDistanceLifecycle = distanceSentTime;
            closestLifecycle = &lifecycle;
        }

        if(smallestDistanceLifecycle > distanceReceivedTime) {
            smallestDistanceLifecycle = distanceReceivedTime;
            closestLifecycle = &lifecycle;
        }
    }

    // search closest packet to mouse
    for(uint32_t packetTypeIndex = NetworkEvent::Type::Connected; packetTypeIndex < NetworkEvent::Type::Last; packetTypeIndex++) {
        auto packetType = static_cast<NetworkEvent::Type>(packetTypeIndex);
        auto packetsOfTypeIt = packets.find(packetType);
        if(packetsOfTypeIt == packets.end())
            continue;

        auto packetsOfType = packetsOfTypeIt->second;
        for(size_t i = 0; i < packetsOfType.timestamps.size(); i++) {
            float timestamp = packetsOfType.timestamps[i];
            PacketSequenceIndex index = packetsOfType.packetIndices[i];
            float dx = timestamp-x;
            float dy = packetTypeIndex-y;
            float sqDist = dx*dx+dy*dy;
            if(sqDist < smallestDistancePacket) {
                smallestDistancePacket = sqDist;
                packetInfo.packetType = packetType;
                packetInfo.timestamp = timestamp;
                packetInfo.sequenceIndex = index;
            }
        }
    }

    constexpr float minDistanceSq = 1*1;
    if(smallestDistanceLifecycle > minDistanceSq) { // needs to be at least close to the mouse pointer!
        closestLifecycle = nullptr;
    }
    if(smallestDistancePacket > minDistanceSq) { // needs to be at least close to the mouse pointer!
        packetInfo.sequenceIndex = 0;
    }

    if(closestLifecycle != nullptr) {
        // make sure the lifecycle is linked to the selected packet
        if(closestLifecycle->sequenceIndex != packetInfo.sequenceIndex) {
            closestLifecycle = nullptr;
        }
    }
    return std::make_pair(packetInfo, closestLifecycle);
}
