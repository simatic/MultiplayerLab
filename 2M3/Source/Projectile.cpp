#include <Projectile.h>
#include <Car.h>
#include "ResourceHolder.h"

Projectile::Projectile(int dmg, sf::Time lifetime, float speed, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, Car* car, const TextureHolder& textures) :
	ProjectileLogic(dmg, lifetime, speed, pos, direction, rect, car),
	mTextures(textures)
{
	setSprite();
}

Projectile::Projectile(int dmg, sf::Time lifetime, float speed, float detection, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, Car* car, const TextureHolder& textures) :
	ProjectileLogic(dmg, lifetime, speed, detection, pos, direction, rect, car),
	mTextures(textures)
{
	setSprite();
}

void Projectile::setSprite()
{
	if (mGuided) { mSprite.setTexture(mTextures.get(Textures::Missile)); mSprite.setScale(sf::Vector2f(1.2f, 1.45f)); }
	else { mSprite.setTexture(mTextures.get(Textures::Bullet)); mSprite.setScale(sf::Vector2f(0.5f, 0.5f)); }

	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2, bounds.height / 2);
}

void Projectile::update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs)
{
	ProjectileLogic::update(dt, entities, newEntities, pairs);

	mSprite.setPosition(mTransform.position);
	mSprite.setRotation(mTransform.rotation);
}

void Projectile::draw(sf::RenderTarget& target)
{
	target.draw(mSprite);
}