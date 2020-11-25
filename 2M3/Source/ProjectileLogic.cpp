#include <ProjectileLogic.h>
#include <Car.h>
#include "Common/Components/Bullet.h"
#include "Common/Systems/BulletSystem.h"

ProjectileLogic::ProjectileLogic(int dmg, sf::Time lifetime, float speed, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, CarLogic* car) :
	OldEntity(pos, rect),
	mCar(car)
{
	Bullet b = Bullet(dmg, speed, lifetime, false, nullptr, 0, 0);
	addComponent<Bullet>(b);

	setVelocity(speed * direction);
	mType = Type::ProjectileType;
}

ProjectileLogic::ProjectileLogic(int dmg, sf::Time lifetime, float speed, float detection, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, CarLogic* car) :
	OldEntity(pos, rect),
	mCar(car)
{
	Bullet b = Bullet(dmg, speed, lifetime, true, nullptr, detection, 0.3);
	addComponent<Bullet>(b);

	setVelocity(speed * direction);
	mType = Type::ProjectileType;
}

int ProjectileLogic::getDamage()
{
	return getComponent<Bullet>()->damage;
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

		otherCar->damage(getDamage());
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
	return getComponent<Bullet>()->guided;
}

void ProjectileLogic::update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs)
{
	BulletSystem::update(dt, this, entities, pairs);
	OldEntity::update(dt, entities, newEntities, pairs);
}