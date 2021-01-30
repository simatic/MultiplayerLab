#pragma once
#include "Common/Components/Component.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

/**
 * @struct Gun component.
 * Useful for shooting bullets, has a pointing direction and a cooldown time.
 */
struct Gun : public IdentifiableComponent<Gun>
{
    Gun(const sf::Vector2f& pointingDirection, sf::Time cooldown);

    sf::Vector2f    pointingDirection;          //!< Pointing direction of the gun.
    sf::Time        cooldown;                   //!< Cooldown time of the gun.
    sf::Time        elapsedTimeSinceLastShot;   //!< Elapsed time since last shot.
};
