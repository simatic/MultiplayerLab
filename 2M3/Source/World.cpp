#include <World.h>
#include <Car.h>
#include <functional>

World::World(sf::RenderTarget& outputTarget, KeyBinding* keys1, KeyBinding* keys2)
	: mTarget(outputTarget)
{
	Player* p1 = new Player(0, keys1, keys2);
	Player* p2 = new Player(1, keys1, keys2);
	mPlayers.push_back(p1);
	mPlayers.push_back(p2);

	mEntities.push_back(p1->getCar());
	mEntities.push_back(p2->getCar());
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
}