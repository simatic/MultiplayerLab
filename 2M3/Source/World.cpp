#include <World.h>
#include <Car.h>
#include <PickUp.h>
#include <functional>

World::World(sf::RenderTarget& outputTarget, KeyBinding* keys1, KeyBinding* keys2, const FontHolder& fonts)
	: mTarget(outputTarget)
	, mTextures()
	, mPlayerOneGUI(fonts)
	, mPlayerTwoGUI(fonts)
	, mWorldWidth(16000.f)
	, mWorldHeight(9000.f)
{
	loadTextures();

	Player* p1 = new Player(0, keys1, keys2, mTextures);
	Player* p2 = new Player(1, keys1, keys2, mTextures);
	mPlayers.push_back(p1);
	mPlayers.push_back(p2);

	mEntities.push_back(p1->getCar());
	mEntities.push_back(p2->getCar());

	/*mEntities.push_back(new PickUp(PickUp::PickUpType::HealthPack, sf::Vector2f(400, 400)));
	mEntities.push_back(new PickUp(PickUp::PickUpType::MissilesAmmo, sf::Vector2f(600, 400)));*/

	mPlayerOneGUI.initialize(p1);
	mPlayerTwoGUI.initialize(p2);
}

void World::update(sf::Time dt)
{
	std::set<Entity::Pair> pairs;
	/*for (auto& ent : mEntities)
	{
		ent->checkCollisions(mEntities, pairs, dt);
	}*/

	for (auto& ent : mEntities)
	{
		ent->update(dt, mEntities, mNewEntities, pairs);
	}
	for (auto& player : mPlayers)
	{
		player->update(dt); // , mNewEntities);
	}

	for (auto& pair : pairs)
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
}
