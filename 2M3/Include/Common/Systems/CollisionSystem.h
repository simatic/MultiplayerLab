#pragma once
#include <SFML/System/Time.hpp>
#include "Common/Systems/System.h"
#include "Entity.h"

class CollisionSystem : public SignedSystem<Transform, Collider>
{
public:
	static void update(const sf::Time& dt, Entity* entity, std::vector<Entity*>& others, std::set<Entity::Pair>& pairs);
};