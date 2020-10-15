#pragma once
#include <SFML/Window.hpp>
#include <imgui.h>

namespace Interface {
    void pollEvents(sf::Window& window);
    void render();
}
