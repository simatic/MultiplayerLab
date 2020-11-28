#include "World.h"
#include "Car.h"
#include "PickUp.h"
#include <functional>
#include <iostream>
#include "Projectile.h"
#include "Wall.h"
#include "OldEntity.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Kinematics.h"

World::World(sf::RenderTarget& outputTarget, KeyBinding* keys, const FontHolder& fonts, bool local)
	: mTarget(outputTarget)
	, mTextures()
	, mPlayerOneGUI(fonts)
	, mPlayerTwoGUI(fonts)
	, mWorldWidth(16000.f)
	, mWorldHeight(9000.f)
	, mPlayerOneKeys(keys)
{
	loadTextures();

	if (local)
	{
		Player* player = new Player(0, keys, mTextures);
		mPlayers.push_back(player);

		Car* car2 = new Car(100, sf::Vector2f(200, 200), sf::RectangleShape(sf::Vector2f(80, 40)), mTextures);

		mEntities.push_back(player->getCar());
		mEntities.push_back(car2);

		mPlayerOneGUI.initialize(player);

		addWalls();
	}
}

void World::addWalls() {
    auto horizontalWall = sf::RectangleShape(sf::Vector2f(mWorldWidth, 10.0f));
    auto verticalWall = sf::RectangleShape(sf::Vector2f(10.0f, mWorldHeight));
    // north
    mEntities.push_back(new Wall(sf::Vector2f(0, -10), horizontalWall, horizontalWall));
    // south
    mEntities.push_back(new Wall(sf::Vector2f(0, mWorldHeight), horizontalWall, horizontalWall));
    // west
    mEntities.push_back(new Wall(sf::Vector2f(-10, 0), verticalWall, verticalWall));
    // east
    mEntities.push_back(new Wall(sf::Vector2f(mWorldWidth, 0), verticalWall, verticalWall));
}

void World::update(sf::Time dt)
{
	for (auto& ent : mEntities)
	{
		ent->update(dt, mEntities, mNewEntities, mPairs);
	}
	for (auto& player : mPlayers)
	{
		player->update(dt); // , mNewEntities);
	}

	auto removeBegin = std::remove_if(mEntities.begin(), mEntities.end(), std::mem_fn(&OldEntity::toRemove));
	mEntities.erase(removeBegin, mEntities.end());

	for (auto& newEnt : mNewEntities)
	{
		mEntities.push_back(newEnt);
	}
	mNewEntities.clear();
	mPairs.clear();
}

void World::clientUpdate(sf::Time dt)
{
	std::set<OldEntity::Pair> tmpPairs = std::set<OldEntity::Pair>(); //fake set of collision pairs, it is used to ignore local collisions
	for (auto& ent : mEntities)
	{
		ent->update(dt, mEntities, mNewEntities, tmpPairs);
	}
	for (auto& player : mPlayers)
	{
		player->update(dt); // , mNewEntities);
	}

	/*auto removeBegin = std::remove_if(mEntities.begin(), mEntities.end(), std::mem_fn(&Entity::toRemove));
	mEntities.erase(removeBegin, mEntities.end());*/

	for (auto& newEnt : mNewEntities)
	{
		if (newEnt->getID() == 0) mToBeAssignedID.push(newEnt);
		mEntities.push_back(newEnt);
	}
	mNewEntities.clear();
	mPairs.clear();
}

void World::draw()
{
	for (auto& player : mPlayers)
	{
		player->draw(mTarget, mEntities);
	}

	mTarget.setView(mTarget.getDefaultView());

	mPlayerOneGUI.updateElements(mTarget, mEntities, getWorldSize());
	mTarget.draw(mPlayerOneGUI);
}

bool World::handleEvent(const sf::Event& event)
{
	return true;
}

sf::Vector2f World::getWorldSize()
{
	return sf::Vector2f(mWorldWidth, mWorldHeight);
}

void World::loadTextures()
{
	mTextures.load(Textures::Car,		"Media/Textures/Car.png");
	mTextures.load(Textures::Bullet,	"Media/Textures/Bullet.png");
	mTextures.load(Textures::Missile,	"Media/Textures/Missile.png");
}

OldEntity* World::getEntityFromId(sf::Uint64 id)
{
	for (auto& ent : mEntities)
	{
		if (ent->getID() == id) return ent;
	}
	for (auto& ent : mNewEntities)
	{
		if (ent->getID() == id) return ent;
	}
	std::cerr << "Error: no entity with such ID : " << id << std::endl;
	exit(EXIT_FAILURE);
}

std::vector<Player*>& World::getPlayers()
{
	return mPlayers;
}

OldEntity* World::getUnassignedEntity()
{
	OldEntity* res = mToBeAssignedID.front();
	mToBeAssignedID.pop();
	return res;
}

void World::addCollision(OldEntity* ent1, OldEntity* ent2)
{
	mPairs.insert(std::minmax(ent1, ent2));
}

void World::createProjectile(sf::Uint64 id, sf::Vector2f pos, sf::Vector2f velocity, Car* creator, bool guided)
{
	sf::Vector2f projDir = unitVector(velocity);

	if (guided)
	{
		Projectile* proj = new Projectile(5, sf::seconds(10), 400, 400, pos, projDir, sf::RectangleShape(sf::Vector2f(30, 10)), creator, mTextures);
		proj->setSprite();
		mNewEntities.push_back(proj);
	}
	else
	{
		Projectile* proj = new Projectile(1, sf::seconds(1), 1500, pos, projDir, sf::RectangleShape(sf::Vector2f(5, 5)), creator, mTextures);
		proj->setSprite();
		mNewEntities.push_back(proj);
	}
}

void World::createCar(sf::Uint64 id, sf::Vector2f pos, sf::Vector2f velocity, sf::Vector2f direction)
{
	Car* car = new Car(100, pos, sf::RectangleShape(sf::Vector2f(80, 40)), mTextures);
	car->setID(id);
	car->getComponent<Kinematics>()->velocity = velocity;
	car->setCarDirection(direction);
	car->setSprite();
	mNewEntities.push_back(car);
}
