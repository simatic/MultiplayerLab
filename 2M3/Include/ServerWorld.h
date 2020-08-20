#pragma once
#include <vector>
#include <Entity.h>
#include <SFML/System.hpp>
#include <Player.h>

class ServerWorld
{
public:
							ServerWorld();
	void					update(sf::Time dt);
	bool					handleEvent(const sf::Event& event);

	sf::Vector2f			getWorldSize();

	Entity*					getEntityFromId(sf::Uint64 id);
	void					setCarInputs(sf::Uint64 id, Inputs inputs);

private:
	std::vector<Entity*>		mEntities;
	std::vector<Player*>		mPlayers;

	std::vector<Entity*>		mNewEntities;

	float						mWorldWidth;
	float						mWorldHeight;
};