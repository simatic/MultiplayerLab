#pragma once
#include <SFML/System/Time.hpp>
#include "Common/Components/Component.h"

struct Bullet : public IdentifiableComponent<Bullet>
{
	unsigned int damage;
	float maxSpeed;
	sf::Time lifetime;
	
	Bullet(const unsigned int damage, const float maxSpeed, const sf::Time& lifetime);
};