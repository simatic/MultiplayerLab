#pragma once
#include <SFML/System/Time.hpp>
#include "Entity.h"

class CollisionSystem
{
public:
	static void update(const sf::Time& dt, Entity* entity, std::vector<Entity*>& others, std::set<Entity::Pair>& pairs);
};