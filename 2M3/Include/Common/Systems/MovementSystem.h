#pragma once
#include <SFML/System/Time.hpp>
#include "Entity.h"

class MovementSystem
{
public:
	static void update(const sf::Time dt, Entity* entity);
};