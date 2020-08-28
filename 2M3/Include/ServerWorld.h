#pragma once
#include <vector>
#include <Entity.h>
#include <SFML/System.hpp>
#include <Player.h>
#include <stack>
#include <NetworkCommon.h>
#include <ClientData.h>
#include <ResourceHolder.h>
#include <ResourceIdentifiers.h>

//struct TimedInputs
//{
//	sf::Uint64 id;
//	sf::Time timestamp;
//	Inputs inputs;
//};

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
	void					update(sf::Time serverTime, sf::Time dt, sf::UdpSocket& socket, std::vector<ClientData>& clients, std::stack<sf::Uint64>& availableIDs);
	bool					handleEvent(const sf::Event& event);

	void					rollback(sf::Time present, sf::Time rollbackDate, sf::UdpSocket& socket, std::vector<ClientData>& clients, std::stack<sf::Uint64>& availableIDs);

	sf::Vector2f			getWorldSize();
	std::vector<Entity*>	getCars();

	Entity*					getEntityFromId(sf::Uint64 id);
	void					setCarInputs(sf::Uint64 id, Inputs inputs, sf::Time t);

	void					createCar(EntityStruct car);

	void					sendWorld(ClientData client, sf::UdpSocket& socket, sf::Uint64 idCar1, sf::Uint64 idCar2);

private:
	std::vector<Entity*>		mEntities;

	std::vector<Entity*>		mNewEntities;
	std::set<Entity::Pair>		mPairs;

	//std::multiset<TimedInputs>	mInputs;
	std::stack<UpdateFrame>		mFrames;

	float						mWorldWidth;
	float						mWorldHeight;

	TextureHolder				mTextures;
};