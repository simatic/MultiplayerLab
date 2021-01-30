#pragma once
#include "Common/Components/Component.h"
#include "Common/Entity.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

/**
 * @struct GUI component.
 * Contains element to display a speedometer for a vehicle.
 */
struct GUI : public IdentifiableComponent<GUI>
{
    GUI(Entity* car);

    Entity*             car = nullptr;          //!< The entity it refers to.
    sf::RectangleShape  mSpeedometer;           //!< The needle of the speedometer.
	sf::CircleShape     mSpeedometerBackground; //!< The background of the speedometer.
};
