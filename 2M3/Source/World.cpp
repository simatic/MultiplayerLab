#include <World.h>
#include <Car.h>
#include <PickUp.h>
#include <functional>
#include <iostream>
#include <Projectile.h>

World::World(sf::RenderTarget& outputTarget, KeyBinding* keys1, KeyBinding* keys2, const FontHolder& fonts, bool local)
	: mTarget(outputTarget)
	, mTextures()
	, mPlayerOneGUI(fonts)
	, mPlayerTwoGUI(fonts)
	, mWorldWidth(16000.f)
	, mWorldHeight(9000.f)
	, mPlayerOneKeys(keys1)
	, mPlayerTwoKeys(keys2)
{
	loadTextures();

	if (local)
	{
		Player* p1 = new Player(0, keys1, mTextures);
		Player* p2 = new Player(1, keys2, mTextures);
		mPlayers.push_back(p1);
		mPlayers.push_back(p2);

		mEntities.push_back(p1->getCar());
		mEntities.push_back(p2->getCar());

		mPlayerOneGUI.initialize(p1);
		mPlayerTwoGUI.initialize(p2);
	}
}

void World::initialize(EntityStruct p1, EntityStruct p2)
{
	//mPlayerCar = new Car(100, sf::Vector2f(850, 450), sf::RectangleShape(sf::Vector2f(80, 40)), keys, textures); from Player constructor
	std::cout << "initializing world with car1 " << p1.id << " and car2 " << p2.id << std::endl;
	Car* car1 = new Car(100, p1.position, sf::RectangleShape(sf::Vector2f(80, 40)), mPlayerOneKeys, mTextures);
	car1->setID(p1.id);
	Car* car2 = new Car(100, p2.position, sf::RectangleShape(sf::Vector2f(80, 40)), mPlayerTwoKeys, mTextures);
	car2->setID(p2.id);

	Player* player1 = new Player(0, car1);
	Player* player2 = new Player(1, car2);
	mPlayers.push_back(player1);
	mPlayers.push_back(player2);

	mEntities.push_back(car1);
	mEntities.push_back(car2);

	mPlayerOneGUI.initialize(player1);
	mPlayerTwoGUI.initialize(player2);
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

	for (auto& pair : mPairs)
	{
		pair.first->onCollision(pair.second);
	}
	auto removeBegin = std::remove_if(mEntities.begin(), mEntities.end(), std::mem_fn(&Entity::toRemove));
	mEntities.erase(removeBegin, mEntities.end());

	for (auto& ent : mEntities)
	{
		ent->cleanUp(getWorldSize(), dt);
	}

	for (auto& newEnt : mNewEntities)
	{
		mEntities.push_back(newEnt);
	}
	mNewEntities.clear();
	mPairs.clear();
}

void World::clientUpdate(sf::Time dt)
{
	std::set<Entity::Pair> tmpPairs = std::set<Entity::Pair>(); //fake set of collision pairs, it is used to ignore local collisions
	for (auto& ent : mEntities)
	{
		ent->update(dt, mEntities, mNewEntities, tmpPairs);
	}
	for (auto& player : mPlayers)
	{
		player->update(dt); // , mNewEntities);
	}

	for (auto& pair : mPairs)
	{
		pair.first->onCollision(pair.second);
	}
	auto removeBegin = std::remove_if(mEntities.begin(), mEntities.end(), std::mem_fn(&Entity::toRemove));
	mEntities.erase(removeBegin, mEntities.end());

	for (auto& ent : mEntities)
	{
		ent->cleanUp(getWorldSize(), dt);
	}

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

	mPlayerTwoGUI.updateElements(mTarget, mEntities, getWorldSize());
	mTarget.draw(mPlayerTwoGUI);
}

bool World::handleEvent(const sf::Event& event)
{
	bool res = true;
	for (auto ent : mEntities)
	{
		res = ent->handleEvent(event) && res;
	}

	return res;
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

Entity* World::getEntityFromId(sf::Uint64 id)
{
	for (auto& ent : mEntities)
	{
		if (ent->getID() == id) return ent;
	}
	/*std::cerr << "Error: no entity with such ID : " << id << std::endl;
	exit(EXIT_FAILURE);*/
	return nullptr;
}

std::vector<Player*>& World::getPlayers()
{
	return mPlayers;
}

Entity* World::getUnassignedEntity()
{
	Entity* res = mToBeAssignedID.front();
	mToBeAssignedID.pop();
	return res;
}

void World::addCollision(Entity* ent1, Entity* ent2)
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
	car->setVelocity(velocity);
	car->setCarDirection(direction);
	car->setSprite();
	mNewEntities.push_back(car);
}
