#include <Car.h>
#include <Utility.h>
#include <iostream>
#include <Projectile.h>
#include <PickUp.h>


Car::Car(const TextureHolder& textures) :
	mHP(1),
	mHpMax(1),
	mDrifting(false),
	mForward(true),
	mPrevDriftingSign(0),
	mCrash(false),
	mDust(sf::Color::Black, sf::Time::Zero),
	mAction(CarAction::ShootBullet),
	mLaunchedMissile(false),
	mMissileAmmo(5),
	mShowMap(false),
	Entity(sf::Vector2f(0, 0), sf::RectangleShape(sf::Vector2f(0, 0)), textures)
{
	mType = Type::CarType;
	Entity::setSprite();
}

Car::Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, KeyBinding* keys, const TextureHolder& textures) :
	mHP(hp),
	mHpMax(hp),
	mKeyBindings(keys),
	mShootDelay(sf::seconds(0.1)),
	mDust(sf::Color::White, sf::seconds(0.7)),
	mCrash(false),
	mAction(CarAction::ShootBullet),
	mLaunchedMissile(false),
	mMissileAmmo(5),
	mShowMap(false),
	Entity(pos, rect, textures)
{
	mType = Type::CarType;
	mCarDirection = sf::Vector2f(1, 0);

	mTires = sf::VertexArray(sf::Lines, 1);
	mTires[0].position = mPosition;
	Entity::setSprite();

	mType = Type::CarType;

	mHpBackgroundBar = sf::RectangleShape(sf::Vector2f(40, 10));
	mHpBackgroundBar.setFillColor(sf::Color(127, 127, 127));
	mHpBackgroundBar.setOrigin(sf::Vector2f(20, 5));
	mHpBar = sf::RectangleShape(sf::Vector2f(40, 10));
	mHpBar.setFillColor(sf::Color::Green);
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

		float accel = 0;
		bool driftBrake = false;
		if (sf::Keyboard::isKeyPressed(mKeyBindings->getAssignedKey(PlayerAction::Accelerate)))
		{
			float f = 1;
			if (!mForward) f = 10;
			accel += f * mCarAcceleration;
		}
		if (sf::Keyboard::isKeyPressed(mKeyBindings->getAssignedKey(PlayerAction::Brake)))
		{
			if (mForward && l > mDriftTheshold && angleSign != 0) driftBrake = true;
			else
			{
				float f = 1;
				if (mForward) f = 10;
				accel -= f * mCarAcceleration;
			}
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
		mDrifting = mForward && l > mDriftTheshold && angleSign != 0 && driftBrake;

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

		sf::Vector2f projDir = mCarDirection;
		if (mDrifting) projDir = rotate(projDir, angleSign * mDriftAngle);

		if (mAction == CarAction::LaunchMissile && mLaunchedMissile)
		{
			mLaunchedMissile = false;
			mMissileAmmo--;

			Projectile* proj = new Projectile(5, sf::seconds(10), 400, 400, mPosition + 25.f * projDir, projDir, sf::RectangleShape(sf::Vector2f(30, 10)), this, getTextures());
			newEntities.push_back(proj);
		}

		if (sf::Keyboard::isKeyPressed(mKeyBindings->getAssignedKey(PlayerAction::DoAction)) && !needsEventInput()) //&& mCurrentShootDelay <= sf::Time::Zero)
		{
			switch (mAction)
			{
			case Car::CarAction::ShootBullet:
			{
				if (mCurrentShootDelay <= sf::Time::Zero)
				{
					mCurrentShootDelay = mShootDelay;

					Projectile* proj = new Projectile(1, sf::seconds(1), 1500, mPosition + 25.f * projDir, projDir, sf::RectangleShape(sf::Vector2f(5, 5)), this, getTextures());
					newEntities.push_back(proj);
				}
				break;
			}
			default:
				break;
			}
		}
	}
	else
	{
	}
}

inline Car::CarAction operator++(Car::CarAction& x)
{
	return x = (Car::CarAction)(((int)(x)+1));
}

bool Car::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == mKeyBindings->getAssignedKey(PlayerAction::ChangeAction))
	{
		++mAction;
		mShowMap = false;
		if (mAction == CarAction::ActionCount) mAction = (CarAction)0;
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == mKeyBindings->getAssignedKey(PlayerAction::DoAction) && needsEventInput())
	{
		switch (mAction)
		{
		case Car::CarAction::LaunchMissile:
		{
			if (!mLaunchedMissile && mMissileAmmo > 0) mLaunchedMissile = true;
			break;
		}
		case Car::CarAction::ToggleMap:
		{
			mShowMap = !mShowMap;
			break;
		}
		default:
			break;
		}
	}

	return true;
}

bool Car::needsEventInput()
{
	bool needs = false;
	switch (mAction)
	{
	case CarAction::ShootBullet:
	{
		needs = false;
		break;
	}
	case CarAction::LaunchMissile:
	{
		needs = true;
		break;
	}
	case CarAction::ToggleMap:
	{
		needs = true;
		break;
	}
	default:
		break;
	}

	return needs;
}

void Car::cleanUp(sf::Vector2f worldSize, sf::Time dt)
{
	if (mPosition.x > worldSize.x || mPosition.x < 0) mPosition.x -= mVelocity.x * dt.asSeconds();
	if (mPosition.y > worldSize.y || mPosition.y < 0) mPosition.y -= mVelocity.y * dt.asSeconds();

	mPrevCollidedWith = mCollidedWith;
	mCollidedWith.clear();
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

	mHpBackgroundBar.setPosition(mPosition + sf::Vector2f(0, 50));
	target.draw(mHpBackgroundBar);
	float hpWidth = mHpBackgroundBar.getSize().x;
	float hpHeight = mHpBackgroundBar.getSize().y;
	float hpBarWidth = hpWidth * mHP / (float)mHpMax;
	mHpBar.setPosition(mHpBackgroundBar.getPosition() - sf::Vector2f(hpWidth / 2.f, hpHeight / 2.f));
	mHpBar.setSize(sf::Vector2f(hpBarWidth, hpHeight));
	target.draw(mHpBar);

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
	std::cout << "took " << points << " dmg" << std::endl;
	if (mHP <= 0) mToRemove = true;
}

void Car::repair(int points)
{
	mHP += points;
	if (mHP > mHpMax) mHP = mHpMax;
}

void Car::addMissileAmmo(int ammo)
{
	mMissileAmmo += ammo;
}

void Car::onCollision(Entity* other)
{
	switch (other->getType())
	{
	case Type::CarType :
	{
		Car* otherCar = dynamic_cast<Car*>(other);
		mCollidedWith.push_back(other);

		bool collision = true;
		for (auto& ent : mPrevCollidedWith)
		{
			if (ent == other)
			{
				collision = false;
				break;
			}
		}

		if (collision)
		{
			std::cout << "collision" << std::endl;
			damage(2);
			otherCar->damage(2);
		}

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

	case Type::PickUpType : 
	{
		PickUp* otherPickup = dynamic_cast<PickUp*>(other);
		otherPickup->onCollision(this);
	}
	default:
		break;
	}
}

std::string Car::getActionText()
{
	std::string res = "null";
	switch (mAction)
	{
	case CarAction::ShootBullet:
	{
		res = "Shoot Bullets";
		break;
	}
	case CarAction::LaunchMissile:
	{
		res = "Launch Missile (x" + std::to_string(mMissileAmmo) + ")";
		break;
	}
	case CarAction::ToggleMap :
	{
		res = "Toggle Map";
		break;
	}
	default:
		break;
	}

	return res;
}

float Car::getSpeedRatio()
{
	return length(mVelocity) / mCarMaxSpeed;
}

bool Car::getShowMap()
{
	return mShowMap;
}
