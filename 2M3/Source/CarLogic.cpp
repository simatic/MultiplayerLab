#include <CarLogic.h>
#include <Utility.h>
#include <iostream>
#include <ProjectileLogic.h>
#include "ResourceHolder.h"
#include "NetworkCommon.h"
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
	mShootDelay(sf::seconds(0.1)),
	mCrash(false),
	mLaunchedMissile(false),
	mMissileAmmo(5),
	mInputs({ false, false, false, false, false, false, false }),
	OldEntity(pos, rect)
{
	CarInput inputs = CarInput();
	addComponent<CarInput>(inputs);

	Health health = Health(hp, hp);
	addComponent<Health>(health);

	CarEngine engine = CarEngine(1000, 1000 / 3, 200, 24, 800, M_PI / 3, 0.001f, sf::Vector2f(1, 0));
	addComponent<CarEngine>(engine);

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

bool CarLogic::handleEvent(const sf::Event& event)
{
	CarInput* inputs = getComponent<CarInput>();
	if (mKeyBindings != nullptr)
	{
		inputs->changeActionEvent = inputs->changeActionEvent || event.type == sf::Event::KeyPressed && event.key.code == mKeyBindings->getAssignedKey(PlayerAction::ChangeAction);
		inputs->doActionEvent = inputs->doActionEvent || event.type == sf::Event::KeyPressed && event.key.code == mKeyBindings->getAssignedKey(PlayerAction::DoAction);
	}

	return true;
}

void CarLogic::cleanUp(sf::Vector2f worldSize, sf::Time dt)
{
	if (getComponent<Transform>()->position.x > worldSize.x || getComponent<Transform>()->position.x < 0) getComponent<Transform>()->position.x -= getVelocity().x * dt.asSeconds();
	if (getComponent<Transform>()->position.y > worldSize.y || getComponent<Transform>()->position.y < 0) getComponent<Transform>()->position.y -= getVelocity().y * dt.asSeconds();

	mPrevCollidedWith = mCollidedWith;
	mCollidedWith.clear();
}

void CarLogic::crash(sf::Vector2f otherVelocity)
{
	mCrash = true;
	//mVelocity = sf::Vector2f(0, 0);
	setVelocity(otherVelocity);
}

void CarLogic::damage(int points)
{
	Health* h = getComponent<Health>();
	h->health -= points;
	std::cout << "took " << points << " dmg" << std::endl;
	if (h->health <= 0) mToRemove = true;
}

void CarLogic::repair(int points)
{
	Health* h = getComponent<Health>();
	h->health += points;
	if (h->health > h->maxHealth) h->health = h->maxHealth;
}

void CarLogic::addMissileAmmo(int ammo)
{
	mMissileAmmo += ammo;
}

sf::Vector2f CarLogic::getCarDirection()
{
	return getComponent<CarEngine>()->direction;
}

float CarLogic::getSpeedRatio()
{
	return length(getVelocity()) / getComponent<CarEngine>()->maxSpeed;
}

Inputs CarLogic::getSavedInputs()
{
	return mInputs;
}

void CarLogic::setInputs(Inputs inputs)
{
	mInputs = inputs;
}

void CarLogic::setCarDirection(sf::Vector2f d)
{
	getComponent<CarEngine>()->direction = d;
}

void CarLogic::instanciateBullet(const sf::Vector2f& position, const sf::Vector2f& direction, std::vector<OldEntity*>& newEntities)
{
	std::cout << "CarLogic" << std::endl;
	ProjectileLogic* proj = new ProjectileLogic(1, sf::seconds(1), 1500, position + 25.f * direction, direction, sf::RectangleShape(sf::Vector2f(5, 5)), this);
	newEntities.push_back(proj); 
}