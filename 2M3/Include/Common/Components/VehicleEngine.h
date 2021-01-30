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
	
	bool 		isGoingForward = false;		//!< If the vehicle is going forward.
	bool 		isDrifting = false;			//!< If the vehicle is drifting.
	float 		previousDriftingSign = 0.f;	//!< Which direction was the vehicle previously drifting.

    float 		maxSpeed;					//!< Vehicle max speed.
	float 		backwardsMaxSpeed;			//!< Vehicle backwards max speed.
	float 		acceleration;				//!< Vehicle acceleration.
	float 		turnRadius;					//!< Vehicle turn radius.
	float 		driftThreshold;				//!< Speed before the vehicle starts to drift.
	float 		driftAngle;					
	float 		drag;

	sf::Vector2f direction;					//!< Direction of the vehicle.
};