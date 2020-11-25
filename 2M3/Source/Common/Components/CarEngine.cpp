#include "Common/Components/CarEngine.h"

CarEngine::CarEngine(const float maxSpeed, const float backwardsMaxSpeed, const float acceleration, 
                     const float turnRadius, const float driftThreshold, const float driftAngle, const float drag,  const sf::Vector2f& direction) :
    maxSpeed(maxSpeed),
    backwardsMaxSpeed(backwardsMaxSpeed),
    acceleration(acceleration),
    turnRadius(turnRadius),
    driftThreshold(driftThreshold),
    driftAngle(driftAngle),
    drag(drag),
    direction(direction)
{}