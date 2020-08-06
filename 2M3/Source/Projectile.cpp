#include <Projectile.h>

Projectile::Projectile(int dmg, sf::Vector2f pos, sf::Vector2f velocity, sf::RectangleShape rect) :
	Entity(pos, rect),
	mDamage(dmg)
{
	mVelocity = velocity;
}