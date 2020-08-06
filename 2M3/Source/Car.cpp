#include <Car.h>
#include <Utility.h>
#include <iostream>


Car::Car() :
	mHP(1),
	mHpMax(1),
	Entity(sf::Vector2f(0, 0), sf::RectangleShape(sf::Vector2f(0, 0)))
{
	
}

Car::Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, bool input) :
	mHP(hp),
	mHpMax(hp),
	mTakesInput(input),
	Entity(pos, rect)
{
	mCarDirection = sf::Vector2f(1, 0);
	tires = sf::VertexArray(sf::Lines, 1);
	tires[0].position = mPosition;
}

void Car::update(sf::Time dt)
{
	if (mTakesInput) getInput(dt);
	Entity::update(dt);
	/*mShape.setPosition(mPosition);
	mShape.setRotation(mRotation);
	mSprite.setRotation(mRotation);

	for (int i = 0; i < tires.getVertexCount(); i++)
	{
		tires[i].position -= mVelocity * dt.asSeconds();
	}*/
	tires.append(sf::Vertex(mPosition - (float)20 * mCarDirection));
	tires.append(sf::Vertex(mPosition - (float)20 * mCarDirection));
}

void Car::getInput(sf::Time dt)
{
	//std::cout << "car input" << std::endl;

	float l = length(mVelocity);
	float accel = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		float f = 1;
		if (!mForward) f = 10;
		accel += f * mCarAcceleration;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		float f = 1;
		if (mForward) f = 10;
		accel -= f * mCarAcceleration;
	}
	if (accel == 0 && l > 200)
	{
		accel = (l * l + 2 * l) * mDrag;
		if (mForward) accel *= -1;
	}
	else if (accel == 0)
	{
		mVelocity = sf::Vector2f(0, 0);
	}

	float angle = 0;
	float angleSign = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && l > 50)
	{
		angle += M_PI / 3;
		angleSign += 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && l > 50)
	{
		angle -= M_PI / 3;
		angleSign -= 1;
	}

	float tangAccel = accel * cos(angle);
	float radAccel = accel * sin(angle);

	sf::Vector2f tangAccelVector = tangAccel * mCarDirection;
	mVelocity += tangAccelVector * dt.asSeconds();
	l = length(mVelocity);
	if (mForward && l > mCarMaxSpeed)
	{
		mVelocity *= mCarMaxSpeed / l;
	}
	else if (!mForward && l > mCarBackwardsMaxSpeed)
	{
		mVelocity *= mCarBackwardsMaxSpeed / l;
	}

	bool prevDrifting = mDrifting;
	mDrifting = mForward && l > mDriftTheshold && angleSign != 0;

	float theta = sqrt(abs(radAccel) / mTurnRadius) * dt.asSeconds();
	mForward = scalar(mVelocity, mCarDirection) >= 0;
	if (!mForward)
	{
		mVelocity = rotate(mVelocity, -theta * angleSign);
		mCarDirection = rotate(mCarDirection, -theta * angleSign);
	}
	else
	{
		mVelocity = rotate(mVelocity, theta * angleSign);
		mCarDirection = rotate(mCarDirection, theta * angleSign);
	}

	if (prevDrifting && !mDrifting)
	{
		mCarDirection = rotate(mCarDirection, mPrevDriftingSign * mDriftAngle);
		mVelocity = rotate(mVelocity, mPrevDriftingSign * mDriftAngle);
	}
	mPrevDriftingSign = angleSign;

	//mPosition += mVelocity * dt.asSeconds();

	/*if (carPos.x > 1600) carPos.x = 0;
	else if (carPos.x < 0) carPos.x = 1600;

	if (carPos.y > 900) carPos.y = 0;
	else if (carPos.y < 0) carPos.y = 900;

	shape.setPosition(carPos);*/

	float carAngle = 0;
	if (mCarDirection.x != 0) carAngle = -atan2(mCarDirection.y, mCarDirection.x);
	if (mCarDirection.x == 0 && mCarDirection.y != 0) carAngle = M_PI_2 * mCarDirection.y / abs(mCarDirection.y);
	if (mDrifting) carAngle += angleSign * mDriftAngle;
	//shape.setRotation(-carAngle * 180.0 / M_PI);
	mRotation = -carAngle * 180.0 / M_PI;
}

void Car::draw(sf::RenderTarget& target)
{
	target.draw(tires);
	Entity::draw(target);
}

void Car::damage(int points)
{
	mHP -= points;
}

void Car::repair(int points)
{
	mHP += points;
	if (mHP > mHpMax) mHP = mHpMax;
}