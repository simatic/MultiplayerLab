#pragma once
#include "Common/Components/Component.h"
#include "Entity.h"

#include <SFML/System/Time.hpp>

struct Bullet : public IdentifiableComponent<Bullet>
{
	Bullet(
		const unsigned int 		damage, 
		const float 			maxSpeed, 
		const sf::Time& 		lifetime,  
		std::shared_ptr<Entity>	owner);
	
	unsigned int 				damage;
	float 						maxSpeed;
	sf::Time 					lifetime;
	std::weak_ptr<Entity> 		owner;
};