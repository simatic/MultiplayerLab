#pragma once
#define _USE_MATH_DEFINES

#include <SFML/System.hpp>
#include "Entity.h"
#include <math.h>

class Car : public Entity
{
public:
	void damage(int points) override;
	void repair(int points) override;

private:
	int mHpMax;

	sf::Vector2f mCarDirection;

	const float mCarMaxSpeed = 1000;
	const float mCarBackwardsMaxSpeed = mCarMaxSpeed / 3;
	const float mCarAcceleration = 200;
	const float mTurnRadius = 12;
	const float mDriftTheshold = mCarMaxSpeed - 200;
	const float mDriftAngle = M_PI / 3;
	const float mDrag = 0.001;

	bool mForward;
	bool mDrifting;
	float mPrevDriftingSign;

};