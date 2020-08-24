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

void ServerWorld::update(sf::Time serverTime, sf::Time dt)
{
	for (auto& ent : mEntities)
	{
		ent->serverUpdate(serverTime, dt, mEntities, mNewEntities, mPairs);
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

bool operator<(const TimedInputs& inputs1, const TimedInputs& inputs2)
{
	return inputs1.timestamp < inputs2.timestamp;
}

void ServerWorld::setCarInputs(sf::Uint64 id, Inputs inputs, sf::Time t)
{
	Entity* entity = getEntityFromId(id);
	Car* car = dynamic_cast<Car*>(entity);
	car->insertInputs(t, inputs);

	//mInputs.insert({ id, t, inputs });
	
}

void ServerWorld::rollback(sf::Time present, sf::Time rollbackDate)
{
	sf::Time current = present;
	std::stack<sf::Time> deltas = std::stack<sf::Time>();

	while (current > rollbackDate)
	{
		UpdateFrame frame = mFrames.top();
		mFrames.pop();

		current -= frame.dt;
		deltas.push(frame.dt);

		for (auto& up : frame.velocityUpdates)
		{
			up.entity->offset(up.entity->getVelocity() * frame.dt.asSeconds());
			up.entity->setVelocity(up.oldVelocity);
		}

		for (auto& up : frame.presenceUpdates)
		{
			if (up.added)
			{
				remove(mEntities.begin(), mEntities.end(), up.entity);
			}
			else
			{
				up.entity->unremove();
				mEntities.push_back(up.entity);
			}
		}
	}

	while (current < present)
	{
		sf::Time dt = deltas.top();
		deltas.pop();

		update(current, dt);
		current += dt;
	}
}
