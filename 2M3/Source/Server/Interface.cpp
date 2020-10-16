//
// Created by jglrxavpok on 15/10/2020.
//
#include "Server/Interface.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <implot.h>
#include <Common/Constants.h>
#include <Server/NetworkSettings.h>

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

void Interface::render() {
    // TODO: foreach client
    if(ImGui::Begin("Contrôles")) {
        ImGui::Text("IP du client: %s", "TODO TODO TODO"); // TODO: ip
        ImGui::Text("Port du client: %i", DEFAULT_PORT); // TODO: port
        ImGui::Separator();

        ImGui::TextColored(TITLE_COLOR, "Pertes de packets");
        ImGui::Text("Pertes de packets venant du client");
        DragFloat("Pourcentage de pertes client", [&]{return NetworkSettings::getInstance()->getPercentageInComingPacketLost();}, [&](float v){NetworkSettings::getInstance()->setPercentageInComingPacketLost(v);});

        ImGui::Text("Pertes de packets partant du serveur");
        DragFloat("Pourcentage de pertes serveur", [&]{return NetworkSettings::getInstance()->getPercentageOutGoingPacketLost();}, [&](float v){NetworkSettings::getInstance()->setPercentageOutGoingPacketLost(v);});

        ImGui::Separator();
        ImGui::TextColored(TITLE_COLOR, "Délais");
        ImGui::Text("Délai avant traitement");
        DragFloat("Délai avant traitement (en secondes)", getTest, setTest, 0, 10.0f);

        ImGui::Text("Avant envoi");
        DragFloat("Délai avant envoi (en secondes)", getTest, setTest, 0, 10.0f);

        ImGui::Separator();

        if(ImGui::TreeNode("Graphes des packets")) {
            if(ImPlot::BeginPlot("Packets entrants")) {
                float timestampsIncoming[] = {0.1f, 0.2f, 0.65f, 0.9f, 1.5f, 2.36f, 3.0f, 3.56f, 4.0f, 5.0f, 6.28f};
                float valuesIncoming[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
                ImPlot::PlotStems("Packets entrants", timestampsIncoming, valuesIncoming, 10);

                float fakeDelay = 0.02f;
                float timestampsOutgoing[] = {0.1f + fakeDelay, 0.2f + fakeDelay, 0.65f + fakeDelay, 0.9f + fakeDelay, 1.5f + fakeDelay, 2.36f + fakeDelay, 3.0f + fakeDelay, 3.56f + fakeDelay, 4.0f + fakeDelay, 5.0f + fakeDelay, 6.28f + fakeDelay};
                float valuesOutgoing[] = {-0,-1,-2,-3,-4,-5,-6,-7,-8,-9};
                ImPlot::PlotStems("Packets sortants", timestampsOutgoing, valuesOutgoing, 10);
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