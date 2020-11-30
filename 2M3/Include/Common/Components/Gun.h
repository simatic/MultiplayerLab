#pragma once
#include "Common/Components/Component.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

struct Gun : public IdentifiableComponent<Gun>
{
    Gun(const sf::Vector2f& pointingDirection, sf::Time cooldown);

    sf::Vector2f    pointingDirection;
    sf::Time        cooldown;
    sf::Time        elapsedTimeSinceLastShot;
};
