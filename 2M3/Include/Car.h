#pragma once
#define _USE_MATH_DEFINES
#include "Entity.h"
#include <math.h>

class Car : public Entity
{
public:
	Car();
	Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, bool input);

	void update(sf::Time dt) override;
	void getInput(sf::Time dt);

	void draw(sf::RenderTarget& target) override;

	void damage(int points);
	void repair(int points);

private:
	int mHP;
	int mHpMax;

	bool mTakesInput;

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

	sf::VertexArray tires;

};