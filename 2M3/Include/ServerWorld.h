#pragma once
#include <vector>
#include <Entity.h>
#include <SFML/System.hpp>
#include <Player.h>
#include <stack>

struct TimedInputs
{
	sf::Uint64 id;
	sf::Time timestamp;
	Inputs inputs;
};

struct UpdatedVelocity
{
	Entity* entity;
	sf::Vector2f oldVelocity;
};

struct UpdatedEntityPresence
{
	Entity* entity;
	bool added;
};

struct UpdateFrame
{
	sf::Time dt;
	std::vector<UpdatedVelocity> velocityUpdates;
	std::vector<UpdatedEntityPresence> presenceUpdates;
};

class ServerWorld
{
public:
							ServerWorld();
	void					update(sf::Time dt);
	bool					handleEvent(const sf::Event& event);

	void					rollback(sf::Time present, sf::Time rollbackDate);

	sf::Vector2f			getWorldSize();

	Entity*					getEntityFromId(sf::Uint64 id);
	void					setCarInputs(sf::Uint64 id, Inputs inputs, sf::Time t);

private:
	std::vector<Entity*>		mEntities;

	std::vector<Entity*>		mNewEntities;
	std::set<Entity::Pair>		mPairs;

	std::multiset<TimedInputs>	mInputs;
	std::stack<UpdateFrame>		mFrames;

	float						mWorldWidth;
	float						mWorldHeight;
};