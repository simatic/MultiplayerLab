#pragma once
#include <Common/Components/Component.h>
#include <Entity.h>

#include <SFML/System/Time.hpp>

/**
 * @struct Bullet component.
 * Holds information to model the behavior of a bullet.
 */
struct Bullet : public IdentifiableComponent<Bullet>
{
	Bullet(
		const unsigned int 		damage, 
		const float 			maxSpeed, 
		const sf::Time& 		lifetime,  
		std::shared_ptr<Entity>	owner);
	
	unsigned int 				damage;		//!< The damage points when hitting a target.
	float 						maxSpeed;	//!< Max speed of the bullet.
	sf::Time 					lifetime;	//!< Lifetime of the bullet.
	std::weak_ptr<Entity> 		owner;		//!< Who launched the bullet.
};