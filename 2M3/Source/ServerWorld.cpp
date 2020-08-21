#include <ServerWorld.h>
#include <Car.h>
#include <PickUp.h>
#include <functional>
#include <iostream>

ServerWorld::ServerWorld()
	: mWorldWidth(16000.f)
	, mWorldHeight(9000.f)
{
}

void ServerWorld::update(sf::Time dt)
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

Entity* ServerWorld::getEntityFromId(sf::Uint64 id)
{
	for (auto& ent : mEntities)
	{
		if (ent->getID() == id) return ent;
	}
	std::cerr << "Error: no entity with such ID : " << id << std::endl;
	exit(EXIT_FAILURE);
}

void ServerWorld::setCarInputs(sf::Uint64 id, Inputs inputs)
{
	Entity* entity = getEntityFromId(id);
	Car* car = dynamic_cast<Car*>(entity);
	car->setInputs(inputs);
}
