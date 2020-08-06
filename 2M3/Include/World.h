#pragma once
#include <vector>
#include <Entity.h>
#include <SFML/System.hpp>
#include <Player.h>

class World
{
public:
	World();
	void					update(sf::Time dt);
	void					draw(sf::RenderTarget& target);

private:
	std::vector<Entity*>		mEntities;
	std::vector<Player*>		mPlayers;
};