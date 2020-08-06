#include <Car.h>
#include <Utility.h>
#include <iostream>


Car::Car() :
	mHP(1),
	mHpMax(1),
	mKeyBindings(1),
	Entity(sf::Vector2f(0, 0), sf::RectangleShape(sf::Vector2f(0, 0)))
{
	
}

Car::Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, KeyBinding keys) :
	mHP(hp),
	mHpMax(hp),
	mKeyBindings(keys),
	mShootDelay(sf::seconds(0.1)),
	Entity(pos, rect)
{
	mCarDirection = sf::Vector2f(1, 0);
	tires = sf::VertexArray(sf::Lines, 1);
	tires[0].position = mPosition;
}

void Car::update(sf::Time dt, std::vector<Entity*>& newEntities)
{
	if (mCurrentShootDelay > sf::Time::Zero) mCurrentShootDelay -= dt;

	getInput(dt, newEntities);

	Entity::update(dt, newEntities);

	tires.append(sf::Vertex(mPosition - (float)20 * mCarDirection));
	tires.append(sf::Vertex(mPosition - (float)20 * mCarDirection));
}

void Car::getInput(sf::Time dt, std::vector<Entity*>& newEntities)
{
	float l = length(mVelocity);
	float accel = 0;
	if (sf::Keyboard::isKeyPressed(mKeyBindings.getAssignedKey(PlayerAction::MoveUp)))
	{
		float f = 1;
		if (!mForward) f = 10;
		accel += f * mCarAcceleration;
	}
	if (sf::Keyboard::isKeyPressed(mKeyBindings.getAssignedKey(PlayerAction::MoveDown)))
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
	if (sf::Keyboard::isKeyPressed(mKeyBindings.getAssignedKey(PlayerAction::MoveLeft)) && l > 50)
	{
		angle += M_PI / 3;
		angleSign += 1;
	}
	if (sf::Keyboard::isKeyPressed(mKeyBindings.getAssignedKey(PlayerAction::MoveRight)) && l > 50)
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

	float carAngle = 0;
	if (mCarDirection.x != 0) carAngle = -atan2(mCarDirection.y, mCarDirection.x);
	if (mCarDirection.x == 0 && mCarDirection.y != 0) carAngle = M_PI_2 * mCarDirection.y / abs(mCarDirection.y);
	if (mDrifting) carAngle += angleSign * mDriftAngle;
	mRotation = -carAngle * 180.0 / M_PI;

	if (sf::Keyboard::isKeyPressed(mKeyBindings.getAssignedKey(PlayerAction::Fire)) && mCurrentShootDelay <= sf::Time::Zero)
	{
		mCurrentShootDelay = mShootDelay;

		sf::Vector2f projDir = mCarDirection;
		if (mDrifting) projDir = rotate(projDir, angleSign * mDriftAngle);

		Projectile* proj = new Projectile(1, mPosition + (float)25 * projDir, (float)2000 * projDir, sf::RectangleShape(sf::Vector2f(5, 5)));
		newEntities.push_back(proj);
	}
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