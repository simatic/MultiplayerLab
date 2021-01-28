#pragma once
#include "Common/Components/Component.h"

#include <SFML/System/Vector2.hpp>

/**
 * @struct VehicleEngine component.
 * Holds information to model the behavior of a VehicleEngine.
 */
struct VehicleEngine : public IdentifiableComponent<VehicleEngine>
{
    VehicleEngine(
		const float 		maxSpeed, 
		const float 		backwardsMaxSpeed, 
		const float 		acceleration, 
        const float 		turnRadius, 
		const float 		driftThreshold, 
		const float 		driftAngle, 
		const float 		drag, 
		const sf::Vector2f& direction);
	
	bool 		isGoingForward = false;
	bool 		isDrifting = false;
	float 		previousDriftingSign = 0.f;

    float 		maxSpeed;
	float 		backwardsMaxSpeed;
	float 		acceleration;
	float 		turnRadius;
	float 		driftThreshold;
	float 		driftAngle;
	float 		drag;

	sf::Vector2f direction;
};