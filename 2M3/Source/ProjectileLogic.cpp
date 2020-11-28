#include <ProjectileLogic.h>
#include <Car.h>
#include "Common/Components/Bullet.h"
#include "Common/Systems/BulletSystem.h"

ProjectileLogic::ProjectileLogic(int dmg, sf::Time lifetime, float speed, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, CarLogic* car) :
	OldEntity(pos, rect)
{
	Bullet b = Bullet(dmg, speed, lifetime, false, car, nullptr, 0, 0);
	addComponent<Bullet>(b);

	getComponent<Kinematics>()->velocity = speed * direction;
	mType = Type::ProjectileType;
}

ProjectileLogic::ProjectileLogic(int dmg, sf::Time lifetime, float speed, float detection, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, CarLogic* car) :
	OldEntity(pos, rect)
{
	Bullet b = Bullet(dmg, speed, lifetime, true, car, nullptr, detection, 0.3);
	addComponent<Bullet>(b);

	getComponent<Kinematics>()->velocity = speed * direction;
	mType = Type::ProjectileType;
}

void ProjectileLogic::update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs)
{
	BulletSystem::update(dt, this, entities, pairs);
	OldEntity::update(dt, entities, newEntities, pairs);
}