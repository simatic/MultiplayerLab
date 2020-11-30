#include "Common/Components/Bullet.h"

Bullet::Bullet(const unsigned int damage, const float maxSpeed, const sf::Time& lifetime, Entity* owner) :
	damage(damage),
	maxSpeed(maxSpeed),
	lifetime(lifetime),
	owner(owner)
{}