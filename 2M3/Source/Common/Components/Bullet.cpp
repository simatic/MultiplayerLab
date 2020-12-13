#include "Common/Components/Bullet.h"

Bullet::Bullet(const unsigned int damage, const float maxSpeed, const sf::Time& lifetime, std::shared_ptr<Entity> owner) :
	damage(damage),
	maxSpeed(maxSpeed),
	lifetime(lifetime),
	owner(std::weak_ptr<Entity>(owner))
{}