#include <ProjectileLogic.h>
#include <Car.h>

ProjectileLogic::ProjectileLogic(int dmg, sf::Time lifetime, float speed, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, CarLogic* car) :
	OldEntity(pos, rect),
	bullet(dmg, speed, lifetime),
	mGuided(false),
	mTarget(nullptr),
	mDetectionRange(0),
	mGuideRate(0),
	mCar(car)
{
	mVelocity = speed * direction;
	mType = Type::ProjectileType;
}

ProjectileLogic::ProjectileLogic(int dmg, sf::Time lifetime, float speed, float detection, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, CarLogic* car) :
	OldEntity(pos, rect),
	bullet(dmg, speed, lifetime),
	mGuided(true),
	mTarget(nullptr),
	mDetectionRange(detection),
	mGuideRate(0.3),
	mCar(car)
{
	mVelocity = speed * direction;
	mType = Type::ProjectileType;
}

int ProjectileLogic::getDamage()
{
	return bullet.damage;
}

void ProjectileLogic::onCollision(OldEntity* other)
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
		CarLogic* otherCar = dynamic_cast<CarLogic*>(other);
		if (otherCar == mCar)
		{
			break;
		}

		otherCar->damage(bullet.damage);
		remove();
		break;
	}

	default:
		break;
	}
}

CarLogic* ProjectileLogic::getCar()
{
	return mCar;
}

bool ProjectileLogic::isGuided()
{
	return mGuided;
}

void ProjectileLogic::update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs)
{
	bullet.lifetime -= dt;
	if (bullet.lifetime < sf::Time::Zero)
	{
		mToRemove = true;
	}
	//mTransform.rotation = mCar->getRotation();

	if (mTarget != nullptr && mTarget->toRemove()) mTarget = nullptr;

	if (mGuided)
	{
		if (mTarget != nullptr)
		{
			sf::Vector2f acceleration = unitVector(mTarget->getPosition() - mTransform.position);
			sf::Vector2f velocity = unitVector(unitVector(mVelocity) + mGuideRate * acceleration) * bullet.maxSpeed;
			setVelocity(velocity);
		}
		else
		{
			float minDist = mDetectionRange;
			OldEntity* target = nullptr;
			for (const auto ent : entities)
			{
				float dist = length(mTransform.position - ent->getPosition());
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
	mTransform.rotation = -toDegrees(angle);

	OldEntity::update(dt, entities, newEntities, pairs);
}