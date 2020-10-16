#pragma once
#include <SFML/Window.hpp>
#include <imgui.h>
#include "Server/ServerNetworkHandling.h"

class Interface {

private:
    static std::map<ClientID, std::vector<NetworkEvent::Event>> clientEvents;

public:
    static void pollEvents(sf::Window& window);
    static void render();
    static void renderClientWindow(const std::string& name, UdpClient& client);

    static void onEvent(UdpClient &client, NetworkEvent::Event event);
};
