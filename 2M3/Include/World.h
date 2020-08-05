#pragma once
#include <vector>
#include <Entity.h>
#include <SFML/System.hpp>

class World
{
public:
	World();
	void					update(sf::Time dt);
	void					draw(sf::RenderTarget& target);

private:
	std::vector<Entity*>		mEntities;
};