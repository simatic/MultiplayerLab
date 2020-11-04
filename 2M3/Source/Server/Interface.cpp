//
// Created by jglrxavpok on 15/10/2020.
//
#include "Server/Interface.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <implot.h>

std::map<ClientID, Interface::CompiledEventsMap> Interface::clientEvents{};
std::map<ClientID, std::vector<Interface::PacketLifecycle>> Interface::clientPacketLifecycles{};

void interfaceThread() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Server Interface");
    ImGui::SFML::Init(window);
    ImPlot::CreateContext();

    sf::Clock deltaClock;
    while(window.isOpen()) {
        Interface::pollEvents(window);

        ImGui::SFML::Update(window, deltaClock.restart());
        Interface::render();

        window.clear();
        ImGui::SFML::Render(window);

        window.display();
    }

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

ImVec4 TITLE_COLOR = ImVec4(1,1,0,1);

sf::Clock interfaceClock{};

void Interface::render() {
    std::vector<std::unique_ptr<UdpClient>>& clients = ServerNetworkHandling::getClients();
    for (int i = 0; i < clients.size(); ++i) {
        auto& client = clients.at(i);
        renderClientWindow("Client #" + std::to_string(i+1), *client);
    }
}

void Interface::renderClientWindow(const std::string& title, UdpClient& client) {
    if(ImGui::Begin(("Contrôles "+title).c_str())) {
        ImGui::Text("IP du client: %s", client.address.toString().c_str());
        ImGui::Text("Port du client: %i", client.port);
        ImGui::Separator();

        if(ImGui::TreeNode("Graphes des packets")) {
            if(ImPlot::BeginPlot("Evénements")) {
                CompiledEventsMap& events = clientEvents[client.id];

                for(unsigned int typeID = 0; typeID < NetworkEvent::Type::Last; typeID++) {
                    auto type = static_cast<NetworkEvent::Type>(typeID);

                    if(events.find(type) == events.end())
                        continue;

                    std::vector<float>& timestamps = events[type].timestamps;
                    std::vector<float>& values = events[type].values;

                    ImPlot::PlotScatter(NetworkEvent::name(type), timestamps.data(), values.data(), values.size());
                }

                std::vector<PacketLifecycle>& packetLives = clientPacketLifecycles[client.id];
                for(auto& lifecycle : packetLives) {
                    float times[2] = { lifecycle.edges.first.x, lifecycle.edges.second.x };
                    float values[2] = { lifecycle.edges.first.y, lifecycle.edges.second.y };
                    ImPlot::PlotLine("", times, values, 2);
                }

                ImPlot::EndPlot();
            }
            ImGui::TreePop();
        }

        ImGui::Separator();

        ImGui::TextColored(TITLE_COLOR, "Pertes de packets");
        ImGui::Text("Pertes de packets venant du client");
        DragFloat("Pourcentage de pertes client", [&]{return client.settings.getPercentageInComingPacketLost();}, [&](float v){client.settings.setPercentageInComingPacketLost(v);});

        ImGui::Text("Pertes de packets partant du serveur");
        DragFloat("Pourcentage de pertes serveur", [&]{return client.settings.getPercentageOutGoingPacketLost();}, [&](float v){client.settings.setPercentageOutGoingPacketLost(v);});

        ImGui::Separator();
        ImGui::TextColored(TITLE_COLOR, "Délais");
        ImGui::Text("Délai avant traitement");
        DragFloat("Délai avant traitement (en secondes)", [&]{return client.settings.getIncomingDelay();}, [&](float v){client.settings.setIncomingDelay(v);}, 0, 10.0f);

        ImGui::Text("Avant envoi");
        DragFloat("Délai avant envoi (en secondes)", [&]{return client.settings.getOutgoingDelay();}, [&](float v){client.settings.setOutgoingDelay(v);}, 0, 10.0f);
    }
    ImGui::End();
}

void Interface::pollEvents(sf::Window& window) {
    sf::Event event{};
    while (window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
        }
    }
}

void Interface::onEvent(UdpClient &client, NetworkEvent::Event event) {
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
    if(event.type == NetworkEvent::Type::PacketDelayed) {
        auto& receivedPacketEvents = eventMap[NetworkEvent::Type::PacketReceived];
        auto it = std::find(receivedPacketEvents.packetIndices.begin(), receivedPacketEvents.packetIndices.end(), event.id);
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
}

void ServerNetworkHandling::triggerEvent(UdpClient &client, NetworkEvent::Event event) {
    Interface::onEvent(client, event);
}