#include <Car.h>
#include <Utility.h>
#include <iostream>
#include <Projectile.h>


Car::Car() :
	mHP(1),
	mHpMax(1),
//	mKeyBindings(1),
	mDrifting(false),
	mForward(true),
	mPrevDriftingSign(0),
	mCrash(false),
	mDust(sf::Color::Black, sf::Time::Zero),
	Entity(sf::Vector2f(0, 0), sf::RectangleShape(sf::Vector2f(0, 0)))
{
	mType = Type::CarType;
}

Car::Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, KeyBinding* keys) :
	mHP(hp),
	mHpMax(hp),
	mKeyBindings(keys),
	mShootDelay(sf::seconds(0.1)),
	mDust(sf::Color::White, sf::seconds(0.7)),
	mCrash(false),
	Entity(pos, rect)
{
	mCarDirection = sf::Vector2f(1, 0);
	mTires = sf::VertexArray(sf::Lines, 1);
	mTires[0].position = mPosition;
	mType = Type::CarType;
}

void Car::update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs)
{
	if (mCurrentShootDelay > sf::Time::Zero) mCurrentShootDelay -= dt;

	getInput(dt, newEntities);

	Entity::update(dt, entities, newEntities, pairs);

	mTires.append(sf::Vertex(mPosition - (float)20 * mCarDirection));
	mTires.append(sf::Vertex(mPosition - (float)20 * mCarDirection));

	mDust.setPosition(mPosition - (float)20 * mCarDirection);
	mDust.update(dt);
}

void Car::getInput(sf::Time dt, std::vector<Entity*>& newEntities)
{
	float l = length(mVelocity);
	if (!mCrash)
	{
		float accel = 0;
		if (sf::Keyboard::isKeyPressed(mKeyBindings->getAssignedKey(PlayerAction::Accelerate)))
		{
			float f = 1;
			if (!mForward) f = 10;
			accel += f * mCarAcceleration;
		}
		if (sf::Keyboard::isKeyPressed(mKeyBindings->getAssignedKey(PlayerAction::Brake)))
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
		if (sf::Keyboard::isKeyPressed(mKeyBindings->getAssignedKey(PlayerAction::TurnLeft)) && l > 50)
		{
			angle += M_PI / 3;
			angleSign += 1;
		}
		if (sf::Keyboard::isKeyPressed(mKeyBindings->getAssignedKey(PlayerAction::TurnRight)) && l > 50)
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
		mDrifting = mForward && l > mDriftTheshold&& angleSign != 0;

		float theta = sqrt(abs(radAccel) / mTurnRadius) * dt.asSeconds();
		mForward = dotProduct(mVelocity, mCarDirection) >= 0;
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

		if (sf::Keyboard::isKeyPressed(mKeyBindings->getAssignedKey(PlayerAction::Fire)) && mCurrentShootDelay <= sf::Time::Zero)
		{
			mCurrentShootDelay = mShootDelay;

			sf::Vector2f projDir = mCarDirection;
			if (mDrifting) projDir = rotate(projDir, angleSign * mDriftAngle);

			Projectile* proj = new Projectile(1, mPosition + (float)25 * projDir, (float)2000 * projDir, sf::RectangleShape(sf::Vector2f(5, 5)), this);
			newEntities.push_back(proj);
		}
	}
	else
	{
	}
}

void Car::crash(sf::Vector2f otherVelocity)
{
	mCrash = true;
	//mVelocity = sf::Vector2f(0, 0);
	mVelocity = otherVelocity;
}

void Car::draw(sf::RenderTarget& target)
{
	target.draw(mTires);
	mDust.draw(target);

	Entity::draw(target);

	//draw hitbox
	/*sf::VertexArray hitbox = sf::VertexArray(sf::Quads, 4);
	for (auto& corner : getRectangle().points)
	{
		hitbox.append(sf::Vertex(corner, sf::Color::Red));
	}
	target.draw(hitbox);*/
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

void Car::onCollision(Entity* other)
{
	switch (other->getType())
	{
	case Type::CarType :
	{
		Car* otherCar = dynamic_cast<Car*>(other);

		damage(2);
		otherCar->damage(2);

		sf::Vector2f prevVelocity = mVelocity;
		//crash(other->getVelocity());
		//otherCar->crash(prevVelocity);

		/*setVelocity(0.8f * otherCar->getVelocity());
		otherCar->setVelocity(0.8f * prevVelocity);*/
		mVelocity = sf::Vector2f(0, 0);
		other->setVelocity(sf::Vector2f(0, 0));

		break;
	}

	case Type::ProjectileType :
	{
		Projectile* otherProj = dynamic_cast<Projectile*>(other);
		if (otherProj->getCar() == this)
		{
			break;
		}

		damage(otherProj->getDamage());
		other->remove();
		break;
	}
	}
}