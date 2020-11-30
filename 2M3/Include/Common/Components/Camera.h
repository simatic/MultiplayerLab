#pragma once
#include "Common/Components/Component.h"
#include "Common/Entity.h"

#include <SFML/Graphics/View.hpp>

struct Camera : public IdentifiableComponent<Camera>
{
    Camera() = default;

    sf::View playerView;
    std::weak_ptr<Entity> player;
};