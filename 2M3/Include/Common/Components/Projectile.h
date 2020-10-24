#pragma once
#include <SFML/System/Time.hpp>
#include "Common/Components/Component.h"

struct Projectile : Component
{
	unsigned int damage;
	sf::Time lifetime;
	
	Projectile(const unsigned int damage, const sf::Time& lifetime);
};