#pragma once
#include <SFML/System/Time.hpp>
#include "Common/Systems/System.h"
#include "OldEntity.h"

class CollisionSystem : public SignedSystem<Transform, Collider>
{
public:
	static void update(const sf::Time& dt, OldEntity* entity, std::vector<OldEntity*>& others, std::set<OldEntity::Pair>& pairs);
};