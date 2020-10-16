#pragma once
#include <SFML/Window.hpp>
#include <imgui.h>
#include "Server/ServerNetworkHandling.h"

namespace Interface {
    void pollEvents(sf::Window& window);
    void render();
    void renderClientWindow(const std::string& name, UdpClient& client);
}
