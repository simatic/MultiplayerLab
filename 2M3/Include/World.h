#pragma once
#include <vector>
#include <Entity.h>
#include <SFML/System.hpp>

class World
{
public:
	void					update(sf::Time);
	void					draw();

private:
	std::vector<Entity>		mEntities;
};