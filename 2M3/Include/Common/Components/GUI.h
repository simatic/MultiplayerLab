#pragma once
#include "Common/Components/Component.h"
#include "Common/Entity.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

struct GUI : public IdentifiableComponent<GUI>
{
    GUI(Entity* car);

    Entity*             car = nullptr;
    sf::RectangleShape  mSpeedometer;
	sf::CircleShape     mSpeedometerBackground;
};
