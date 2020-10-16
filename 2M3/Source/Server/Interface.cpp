//
// Created by jglrxavpok on 15/10/2020.
//
#include "Server/Interface.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <implot.h>

std::map<ClientID, std::vector<NetworkEvent::Event>> Interface::clientEvents{};

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
    if(ImGui::DragFloat(label, &value, (max-min)/100.0f, min, max)) {
        set(value);
    }
}

float tmp = 0.0f;
void setTest(float v) {
    tmp = v;
}

float getTest() {
    return tmp;
}

ImVec4 TITLE_COLOR = ImVec4(1,1,0,1);

sf::Clock interfaceClock{};

void Interface::render() {
    std::vector<UdpClient>& clients = ServerNetworkHandling::getClients();
    for (int i = 0; i < clients.size(); ++i) {
        auto& client = clients.at(i);
        renderClientWindow("Client #" + std::to_string(i+1), client);
    }
}

void Interface::renderClientWindow(const std::string& title, UdpClient& client) {
    if(ImGui::Begin(("Contrôles "+title).c_str())) {
        ImGui::Text("IP du client: %s", client.address.toString().c_str());
        ImGui::Text("Port du client: %i", client.port);
        ImGui::Separator();

        ImGui::TextColored(TITLE_COLOR, "Pertes de packets");
        ImGui::Text("Pertes de packets venant du client");
        DragFloat("Pourcentage de pertes client", [&]{return client.settings.getPercentageInComingPacketLost();}, [&](float v){client.settings.setPercentageInComingPacketLost(v);});

        ImGui::Text("Pertes de packets partant du serveur");
        DragFloat("Pourcentage de pertes serveur", [&]{return client.settings.getPercentageOutGoingPacketLost();}, [&](float v){client.settings.setPercentageOutGoingPacketLost(v);});

        ImGui::Separator();
        ImGui::TextColored(TITLE_COLOR, "Délais");
        ImGui::Text("Délai avant traitement");
        DragFloat("Délai avant traitement (en secondes)", getTest, setTest, 0, 10.0f);

        ImGui::Text("Avant envoi");
        DragFloat("Délai avant envoi (en secondes)", getTest, setTest, 0, 10.0f);

        ImGui::Separator();

        if(ImGui::TreeNode("Graphes des packets")) {
            if(ImPlot::BeginPlot("Evénements")) {
                std::vector<NetworkEvent::Event> events = clientEvents[client.id];

                for(unsigned int typeID = 0; typeID < NetworkEvent::Type::Last; typeID++) {
                    auto type = static_cast<NetworkEvent::Type>(typeID);

                    std::vector<float> timestamps{};
                    std::vector<float> values{};

                    for(auto& event : events) {
                        float x = event.timestamp.asSeconds();
                        if(event.type == type) {
                            timestamps.push_back(x);
                            values.push_back(typeID);
                        }
                    }
                    ImPlot::PlotScatter(NetworkEvent::name(type), timestamps.data(), values.data(), values.size());
                }

                ImPlot::EndPlot();
            }
            ImGui::TreePop();
        }
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
        clientEvents[client.id] = std::vector<NetworkEvent::Event>();
    }
    clientEvents[client.id].push_back(event);
}

void ServerNetworkHandling::triggerEvent(UdpClient &client, NetworkEvent::Event event) {
    Interface::onEvent(client, event);
}