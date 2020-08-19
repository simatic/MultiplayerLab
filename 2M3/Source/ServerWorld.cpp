#include <ServerWorld.h>
#include <Car.h>
#include <PickUp.h>
#include <functional>

ServerWorld::ServerWorld()
	: mWorldWidth(16000.f)
	, mWorldHeight(9000.f)
{
}

void ServerWorld::update(sf::Time dt)
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

bool ServerWorld::handleEvent(const sf::Event& event)
{
	bool res = true;
	for (auto ent : mEntities)
	{
		res = ent->handleEvent(event) && res;
	}

	return res;
}

sf::Vector2f ServerWorld::getWorldSize()
{
	return sf::Vector2f(mWorldWidth, mWorldHeight);
}