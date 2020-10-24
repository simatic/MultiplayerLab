#include "Common/Components/Projectile.h"

Projectile::Projectile(const unsigned int damage, const sf::Time& lifetime) :
	damage(damage),
	lifetime(lifetime)
{}