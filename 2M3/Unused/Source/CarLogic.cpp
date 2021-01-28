#include <CarLogic.h>
#include <Utility.h>
#include <iostream>
#include <ProjectileLogic.h>
#include "ResourceHolder.h"
#include "Common/Components/Health.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/CarInput.h"
#include "Common/Systems/KeyboardInputSystem.h"
#include "Common/Systems/CarMovementSystem.h"
#include "Common/Systems/CarCollisionHandling.h"
#include "Common/Systems/GunSystem.h"
#include "Common/Systems/CarDeath.h"
#include <math.h>


CarLogic::CarLogic() :
	CarLogic(1, sf::Vector2f(0, 0), sf::RectangleShape(sf::Vector2f(0, 0)))
{}

CarLogic::CarLogic(int hp, sf::Vector2f pos, sf::RectangleShape rect) :
	CarLogic(hp, pos, rect, nullptr)
{}

CarLogic::CarLogic(int hp, sf::Vector2f pos, sf::RectangleShape rect, KeyBinding* keys) :
	mKeyBindings(keys),
	OldEntity(pos, rect)
{
	CarInput inputs = CarInput();
	addComponent<CarInput>(inputs);

	Health health = Health(hp, hp);
	addComponent<Health>(health);

	VehicleEngine engine = VehicleEngine(1000, 1000 / 3, 200, 24, 800, M_PI / 3, 0.001f, sf::Vector2f(1, 0));
	addComponent<VehicleEngine>(engine);

	Gun gun = Gun(sf::Vector2f(1, 0), sf::seconds(0.1));
	addComponent<Gun>(gun);

	mType = Type::CarType;

	mInstanciateMissile = [this](sf::Vector2f position, sf::Vector2f direction) -> ProjectileLogic* 
	{ 
		return new ProjectileLogic(5, sf::seconds(10), 400, 400, position + 25.f * direction, direction, sf::RectangleShape(sf::Vector2f(30, 10)), this); 
	};

	mInstanciateBullet = [this](sf::Vector2f position, sf::Vector2f direction) -> ProjectileLogic*
	{
		return new ProjectileLogic(1, sf::seconds(1), 1500, position + 25.f * direction, direction, sf::RectangleShape(sf::Vector2f(5, 5)), this);
	};
}

void CarLogic::update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs)
{

	KeyboardInputSystem::update(this, mKeyBindings);
	CarMovementSystem::update(dt, this, newEntities);
	GunSystem::update(dt, this, newEntities);
	OldEntity::update(dt, entities, newEntities, pairs);
	CarCollisionHandling::update(this);
	CarDeath::update(this);
}

void CarLogic::instanciateBullet(const sf::Vector2f& position, const sf::Vector2f& direction, std::vector<OldEntity*>& newEntities)
{
	std::cout << "CarLogic" << std::endl;
	ProjectileLogic* proj = new ProjectileLogic(1, sf::seconds(1), 1500, position + 25.f * direction, direction, sf::RectangleShape(sf::Vector2f(5, 5)), this);
	newEntities.push_back(proj); 
}