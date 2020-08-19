#pragma once
#include <vector>
#include <Entity.h>
#include <SFML/System.hpp>

class ServerWorld
{
public:
							ServerWorld();
	void					update(sf::Time dt);
	bool					handleEvent(const sf::Event& event);

	sf::Vector2f			getWorldSize();

private:
	std::vector<Entity*>		mEntities;
	std::vector<Player*>		mPlayers;

	std::vector<Entity*>		mNewEntities;

	float						mWorldWidth;
	float						mWorldHeight;
};