//
// Created by jglrxavpok on 15/10/2020.
//
#include "Server/Interface.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <implot.h>
#include <Server/Network/ServerClock.h>
#include <Server/Network/DelayCreation.h>

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
static float MAX_DELAY = 5.0f;

void Interface::render(sf::RenderWindow& window, float clientWidth, float interfaceHeight, float startY) {
    if(!implotInit) {
        ImGui::SFML::Init(window);
        ImPlot::CreateContext();

        const ImVec4 colors[] = {
                ImVec4(0.0f, 0.0f, 1.0f, 1.0f), // server
                ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // client 0
                ImVec4(1.0f, 0.0f, 0.0f, 1.0f), // links from client 0
                ImVec4(0.0f, 1.0f, 0.0f, 1.0f), // client 1
                ImVec4(0.0f, 1.0f, 0.0f, 1.0f), // links from client 1
        };
        ImPlot::SetColormap(colors, 5);

        ImPlot::GetStyle().Colors[ImPlotCol_FrameBg] = ImVec4(0,0,0,0);

        ImGui::GetStyle().WindowRounding = 0.0f;
        ImGui::GetStyle().WindowBorderSize = 0.0f;
        implotInit = true;
    }

    ImGui::SFML::Update(window, deltaClock.restart());

    auto& clients = serverNetwork.getClients();

    if(clients.empty()) {
        ImGui::SetNextWindowPos(ImVec2{clientWidth, startY + interfaceHeight / 2.0f});
        if(ImGui::Begin("Waiting clients", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)) {
            ImGui::Text("Client windows will appear in this area once connected.");
        }
        ImGui::End();
    } else {
        auto flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
        ImGui::SetNextWindowPos(ImVec2{0, startY});
        ImGui::SetNextWindowSize(ImVec2{clientWidth*2, interfaceHeight});
        if(ImGui::Begin("Graphes", nullptr, flags)) {
            // TODO: Réajouter la possibilité de passer la souris sur un packet

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

    ImGui::Checkbox("Pause scrolling", &pauseGraphScroll);

    float time = ServerClock::getInstance().asSeconds();

    // pause? => ImGuiCond_Once
    ImPlot::SetNextPlotLimitsX(time-5*zoomFactor, time, !pauseGraphScroll ? ImGuiCond_Always : ImGuiCond_Once);
    ImPlot::SetNextPlotLimitsY(-2, 2, ImGuiCond_Always);

    auto yFlags = ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels;
    // TODO: merge code with incoming?
    if(ImPlot::BeginPlot("Incoming packets", "Time (in seconds)", nullptr, ImVec2(-1,0), ImPlotFlags_None, ImPlotAxisFlags_None, yFlags)) {
        if(ImPlot::IsPlotHovered()) {
            zoomFactor -= ImGui::GetIO().MouseWheel/10.0f * zoomFactor;

            zoomFactor = std::min(100.0f, std::max(0.01f, zoomFactor));
        }

        // TODO: window data to avoid FPS drops
        float leftTime = ImPlot::GetPlotLimits().X.Min;
        float rightTime = ImPlot::GetPlotLimits().X.Max;

        {
            std::lock_guard lk(linkAccess);
            std::size_t leftSideOfWindow = findLeftSideOfWindow(leftTime, afterDelayTimestamps);
            std::size_t rightSideOfWindow = findRightSideOfWindow(rightTime, afterDelayTimestamps);

            ImPlot::PlotScatterG("Packets sent by the server", [](void *ptr, int index) {
                auto *timestamps = static_cast<float *>(ptr);
                float timestamp = timestamps[index];
                return ImPlotPoint(timestamp, 0);
            }, afterDelayTimestamps.data()+leftSideOfWindow, rightSideOfWindow-leftSideOfWindow);
        }

        if(serverNetwork.getClients().size() > 0) {
            auto& client0Packets = clientEvents[serverNetwork.getClients()[0]->id];

            {
                std::lock_guard lk(linkAccess);
                std::size_t leftSideOfWindow = findLeftSideOfWindow(leftTime, client0Packets.receivedTimestamps);
                std::size_t rightSideOfWindow = findRightSideOfWindow(rightTime, client0Packets.receivedTimestamps);

                ImPlot::PlotScatterG("Packets sent by client 1", [](void* ptr, int index) {
                    auto* timestamps = static_cast<float*>(ptr);
                    float timestamp = timestamps[index];
                    return ImPlotPoint(timestamp, 1);
                }, client0Packets.receivedTimestamps.data()+leftSideOfWindow, rightSideOfWindow-leftSideOfWindow);
            }

            // render packet links
            {
                std::lock_guard lk(linkAccess);
                std::size_t leftSideOfWindow = findLeftSideOfWindow(leftTime, client0Packets.receptionLinks);
                std::size_t rightSideOfWindow = findRightSideOfWindow(rightTime, client0Packets.receptionLinks);
                for (std::size_t i = leftSideOfWindow; i < rightSideOfWindow; i++) {
                    auto& link = client0Packets.receptionLinks[i];
                    float xs[2] = {link.timestampA, link.timestampB};
                    float ys[2] = {1, 0}; // client->server
                    ImPlot::PlotLine("Packet trips from client 1", xs, ys, 2);
                }
            }

            if(serverNetwork.getClients().size() > 1) {
                auto& client1Packets = clientEvents[serverNetwork.getClients()[1]->id];

                {
                    std::lock_guard lk(linkAccess);
                    std::size_t leftSideOfWindow = findLeftSideOfWindow(leftTime, client1Packets.receivedTimestamps);
                    std::size_t rightSideOfWindow = findRightSideOfWindow(rightTime, client1Packets.receivedTimestamps);

                    ImPlot::PlotScatterG("Packets sent by client 2", [](void *ptr, int index) {
                        auto *timestamps = static_cast<float *>(ptr);
                        float timestamp = timestamps[index];
                        return ImPlotPoint(timestamp, -1);
                    }, client1Packets.receivedTimestamps.data()+leftSideOfWindow, rightSideOfWindow-leftSideOfWindow);
                }

                {
                    std::lock_guard lk(linkAccess);
                    std::size_t leftSideOfWindow = findLeftSideOfWindow(leftTime, client1Packets.receptionLinks);
                    std::size_t rightSideOfWindow = findRightSideOfWindow(rightTime, client1Packets.receptionLinks);
                    for (std::size_t i = leftSideOfWindow; i < rightSideOfWindow; i++) {
                        auto& link = client1Packets.receptionLinks[i];
                        float xs[2] = {link.timestampA, link.timestampB};
                        float ys[2] = {-1, 0}; // client->server
                        ImPlot::PlotLine("Packet trips from client 2", xs, ys, 2);
                    }
                }
            }
        }
        ImPlot::EndPlot();
    }
}

void Interface::renderOutgoingGraph() {
    static bool pauseGraphScroll = false;
    static float zoomFactor = 0.015f;

    ImGui::Checkbox("Pause scrolling", &pauseGraphScroll);

    float time = ServerClock::getInstance().asSeconds();

    // pause? => ImGuiCond_Once
    ImPlot::SetNextPlotLimitsX(time-5*zoomFactor, time, !pauseGraphScroll ? ImGuiCond_Always : ImGuiCond_Once);
    ImPlot::SetNextPlotLimitsY(-2, 2, ImGuiCond_Always);

    auto yFlags = ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels;

    // TODO: merge code with outgoing?
    if(ImPlot::BeginPlot("Outgoing packets", "Time (in seconds)", nullptr, ImVec2(-1,0), ImPlotFlags_None, ImPlotAxisFlags_None, yFlags)) {
        if(ImPlot::IsPlotHovered()) {
            zoomFactor -= ImGui::GetIO().MouseWheel/10.0f;

            zoomFactor = std::min(100.0f, std::max(0.01f, zoomFactor));
        }

        float leftTime = ImPlot::GetPlotLimits().X.Min;
        float rightTime = ImPlot::GetPlotLimits().X.Max;

        // TODO: window data to avoid FPS drops
        {
            std::lock_guard lk(linkAccess);
            std::size_t leftSideOfWindow = findLeftSideOfWindow(leftTime, sendingStartsTimestamps);
            std::size_t rightSideOfWindow = findRightSideOfWindow(rightTime, sendingStartsTimestamps);

            ImPlot::PlotScatterG("Packets sent by the server", [](void *ptr, int index) {
                auto *timestamps = static_cast<float *>(ptr);
                float timestamp = timestamps[index];
                return ImPlotPoint(timestamp, 0);
            }, sendingStartsTimestamps.data()+leftSideOfWindow, rightSideOfWindow-leftSideOfWindow);
        }

        if(serverNetwork.getClients().size() > 0) {
            auto& client0Packets = clientEvents[serverNetwork.getClients()[0]->id];

            {
                std::lock_guard lk(linkAccess);
                std::size_t leftSideOfWindow = findLeftSideOfWindow(leftTime, client0Packets.sentTimestamps);
                std::size_t rightSideOfWindow = findRightSideOfWindow(rightTime, client0Packets.sentTimestamps);

                ImPlot::PlotScatterG("Packets sent by client 1", [](void *ptr, int index) {
                    auto *timestamps = static_cast<float *>(ptr);
                    float timestamp = timestamps[index];
                    return ImPlotPoint(timestamp, 1);
                }, client0Packets.sentTimestamps.data()+leftSideOfWindow, rightSideOfWindow-leftSideOfWindow);
            }

            {
                std::lock_guard lk(linkAccess);
                // render packet links
                std::size_t leftSideOfWindow = findLeftSideOfWindow(leftTime, client0Packets.transmissionLinks);
                std::size_t rightSideOfWindow = findRightSideOfWindow(rightTime, client0Packets.transmissionLinks);
                for (std::size_t i = leftSideOfWindow; i < rightSideOfWindow; i++) {
                    auto& link = client0Packets.transmissionLinks[i];
                    float xs[2] = {link.timestampA, link.timestampB};
                    float ys[2] = {0, 1}; // client->server
                    ImPlot::PlotLine("Packet trips from client 1", xs, ys, 2);
                }
            }

            if(serverNetwork.getClients().size() > 1) {
                auto& client1Packets = clientEvents[serverNetwork.getClients()[1]->id];
                {
                    std::lock_guard lk(linkAccess);

                    std::size_t leftSideOfWindow = findLeftSideOfWindow(leftTime, client1Packets.sentTimestamps);
                    std::size_t rightSideOfWindow = findRightSideOfWindow(rightTime, client1Packets.sentTimestamps);
                    ImPlot::PlotScatterG("Packets sent by client 2", [](void *ptr, int index) {
                        auto *timestamps = static_cast<float *>(ptr);
                        float timestamp = timestamps[index];
                        return ImPlotPoint(timestamp, -1);
                    }, client1Packets.sentTimestamps.data()+leftSideOfWindow, rightSideOfWindow-leftSideOfWindow);
                }

                {
                    std::lock_guard lk(linkAccess);

                    std::size_t leftSideOfWindow = findLeftSideOfWindow(leftTime, client1Packets.transmissionLinks);
                    std::size_t rightSideOfWindow = findRightSideOfWindow(rightTime, client1Packets.transmissionLinks);
                    for (std::size_t i = leftSideOfWindow; i < rightSideOfWindow; i++) {
                        auto& link = client1Packets.transmissionLinks[i];
                        float xs[2] = {link.timestampA, link.timestampB};
                        float ys[2] = {0, -1}; // client->server
                        ImPlot::PlotLine("Packet trips from client 2", xs, ys, 2);
                    }
                }
            }
        }

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
        ImGui::TextColored(TITLE_COLOR, "Client #%zu", i+1);

        auto& client = *clients[i];
        ImGui::SetNextItemWidth(width/4.0f);
        DragFloat("Incoming packet loss", [&]{return client.settings.getPercentageInComingPacketLost();}, [&](float v){client.settings.setPercentageInComingPacketLost(v);});

        ImGui::SetNextItemWidth(width/4.0f);
        DragFloat("Delay before receiving", [&]{return client.settings.getIncomingDelay();}, [&](float v){client.settings.setIncomingDelay(v);}, 0, MAX_DELAY);
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
        ImGui::TextColored(TITLE_COLOR, "Client #%zu", i+1);

        auto& client = *clients[i];
        ImGui::SetNextItemWidth(width/4.0f);
        DragFloat("Outgoing packet loss", [&]{return client.settings.getPercentageOutGoingPacketLost();}, [&](float v){client.settings.setPercentageOutGoingPacketLost(v);});

        ImGui::SetNextItemWidth(width/4.0f);
        DragFloat("Delay before sending", [&]{return client.settings.getOutgoingDelay();}, [&](float v){client.settings.setOutgoingDelay(v);}, 0, MAX_DELAY);
        ImGui::EndChild();
    }

    ImGui::EndChild();
}

std::size_t Interface::findLeftSideOfWindow(float time, const std::vector<float>& timestamps) {
    auto lower = std::upper_bound(timestamps.begin(), timestamps.end(), time);
    if(lower != timestamps.end()) {
        return std::distance(timestamps.begin(), lower);
    }
    return 0;
}

std::size_t Interface::findRightSideOfWindow(float time, const std::vector<float>& timestamps) {
    auto upper = std::lower_bound(timestamps.begin(), timestamps.end(), time);
    if(upper != timestamps.end()) {
        return std::distance(timestamps.begin(), upper);
    }
    return timestamps.size();
}

std::size_t Interface::findLeftSideOfWindow(float time, const std::vector<PacketLifecycle>& timestamps) {
    auto lower = std::upper_bound(timestamps.begin(), timestamps.end(), time, [](const float& t, const auto& l) { return l.timestampA > t; });
    if(lower != timestamps.end()) {
        return std::distance(timestamps.begin(), lower);
    }
    return 0;
}

std::size_t Interface::findRightSideOfWindow(float time, const std::vector<PacketLifecycle>& timestamps) {
    auto upper = std::lower_bound(timestamps.begin(), timestamps.end(), time, [](const auto& l, const float& t) { return l.timestampB < t; });
    if(upper != timestamps.end()) {
        return std::distance(timestamps.begin(), upper);
    }
    return timestamps.size();
}

void Interface::onEvent(const UdpClient &client, NetworkEvent::Event event) {
    if(clientEvents.find(client.id) == clientEvents.end()) {
        clientEvents[client.id] = {};
    }
    auto& compilation = clientEvents[client.id];

    float time = event.timestamp.asSeconds();

    std::lock_guard lk(linkAccess);
    switch(event.type) {
        case NetworkEvent::PacketReceived: {
            compilation.receivedTimestamps.push_back(time);
            compilation.receivedPacketIndices.push_back(event.id);
        } break;

        case NetworkEvent::PacketDelayed: {
            afterDelayTimestamps.push_back(time);
            afterDelayPacketIndices.push_back(event.id);

            auto receivedPacketLocation = std::find(compilation.receivedPacketIndices.rbegin(), compilation.receivedPacketIndices.rend(), event.id);
            auto receivedPacketIndex = compilation.receivedPacketIndices.size()-std::distance(compilation.receivedPacketIndices.rbegin(), receivedPacketLocation)-1;
            auto receivedPacketTime = compilation.receivedTimestamps[receivedPacketIndex];
            compilation.receptionLinks.push_back({
                event.id,
                receivedPacketTime,
                time,
            });
        } break;

        case NetworkEvent::SendingPacket: {
            sendingStartsTimestamps.push_back(time);
            sendingStartsPacketIndices.push_back(event.id);
        } break;

        case NetworkEvent::SentPacket: {
            compilation.sentTimestamps.push_back(time);
            compilation.sentPacketIndices.push_back(event.id);

            auto sendingPacketLocation = std::find(sendingStartsPacketIndices.rbegin(), sendingStartsPacketIndices.rend(), event.id);
            auto sendingPacketIndex = sendingStartsPacketIndices.size()-std::distance(sendingStartsPacketIndices.rbegin(), sendingPacketLocation)-1;
            auto sendingPacketTime = sendingStartsTimestamps[sendingPacketIndex];
            compilation.transmissionLinks.push_back({
                                                         event.id,
                                                         sendingPacketTime,
                                                         time,
                                                 });
        } break;
    }
}
