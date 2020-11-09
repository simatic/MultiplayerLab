#pragma once
#include <SFML/System/Time.hpp>
#include "Common/Systems/System.h"
#include "Entity.h"

class MovementSystem : public SignedSystem<Transform>
{
public:
	static void update(const sf::Time dt, Entity* entity);
};