#pragma once
#include <SFML/System/Time.hpp>
#include "Common/Components/Component.h"
#include "Entity.h"

struct Bullet : public IdentifiableComponent<Bullet>
{
	Bullet(const unsigned int damage, const float maxSpeed, const sf::Time& lifetime, const bool guided, Entity* owner, Entity* target, const float detectionRange, const float guideRate);
	
	unsigned int damage;
	float maxSpeed;
	sf::Time lifetime;
	bool guided;
	Entity* owner;
	Entity* target;
	float detectionRange;
	float guideRate;
};