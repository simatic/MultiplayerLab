#include "Common/Components/Gun.h"

Gun::Gun(const sf::Vector2f& pointingDirection, sf::Time cooldown) :
    pointingDirection(pointingDirection),
    cooldown(cooldown),
    elapsedTimeSinceLastShot(cooldown)
{}