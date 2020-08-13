#include <Projectile.h>
#include <Car.h>

Projectile::Projectile(int dmg, sf::Time lifetime, float speed, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, Car* car) :
	Entity(pos, rect),
	mDamage(dmg),
	mLifetime(lifetime),
	mMaxSpeed(speed),
	mGuided(false),
	mTarget(nullptr),
	mDetectionRange(0),
	mGuideRate(0),
	mCar(car)
{
	mVelocity = speed * direction;
	mType = Type::ProjectileType;
}

Projectile::Projectile(int dmg, sf::Time lifetime, float speed, float detection, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, Car* car) :
	Entity(pos, rect),
	mDamage(dmg),
	mLifetime(lifetime),
	mMaxSpeed(speed),
	mGuided(true),
	mTarget(nullptr),
	mDetectionRange(detection),
	mGuideRate(0.3),
	mCar(car)
{
	mVelocity = speed * direction;
	mType = Type::ProjectileType;
}

int Projectile::getDamage()
{
	return mDamage;
}

void Projectile::onCollision(Entity* other)
{
	switch (other->getType())
	{
	case Type::ProjectileType:
	{
		remove();
		other->remove();
		break;
	}

	case Type::CarType:
	{
		Car* otherCar = dynamic_cast<Car*>(other);
		if (otherCar == mCar)
		{
			break;
		}

		otherCar->damage(mDamage);
		remove();
		break;
	}
	}
}

Car* Projectile::getCar()
{
	return mCar;
}

void Projectile::update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs)
{
	mLifetime -= dt;
	if (mLifetime < sf::Time::Zero)
	{
		mToRemove = true;
	}

	if (mGuided)
	{
		if (mTarget != nullptr)
		{
			sf::Vector2f acceleration = unitVector(mTarget->getPosition() - mPosition);
			sf::Vector2f velocity = unitVector(unitVector(mVelocity) + mGuideRate * acceleration) * mMaxSpeed;
			setVelocity(velocity);
		}
		else
		{
			float minDist = mDetectionRange;
			Entity* target = nullptr;
			for (const auto ent : entities)
			{
				float dist = length(mPosition - ent->getPosition());
				if (dist < minDist && ent->getType() == Type::CarType && ent != mCar)
				{
					minDist = dist;
					target = ent;
				}
			}
			mTarget = target;
		}
	}

	float angle = 0;
	if (mVelocity.x != 0) angle = -atan2(mVelocity.y, mVelocity.x);
	if (mVelocity.x == 0 && mVelocity.y != 0) angle = M_PI_2 * mVelocity.y / abs(mVelocity.y);
	mRotation = -toDegrees(angle);

	Entity::update(dt, entities, newEntities, pairs);
}