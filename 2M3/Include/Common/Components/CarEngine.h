#pragma once
#include <math.h>
#include "Common/Components/Component.h"

struct CarEngine : public IdentifiableComponent<CarEngine>
{
    CarEngine(const float maxSpeed, const float backwardsMaxSpeed, const float acceleration, 
              const float turnRadius, const float driftThreshold, const float driftAngle, const float drag);

    float maxSpeed;
	float backwardsMaxSpeed;
	float acceleration;
	float turnRadius;
	float driftThreshold;
	float driftAngle;
	float drag;
};