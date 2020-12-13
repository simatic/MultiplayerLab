#include <Projectile.h>
#include <Car.h>
#include "ResourceHolder.h"
#include "Common/Components/Sprite.h"
#include "Common/Systems/RenderSystem.h"

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
	sf::Sprite s = sf::Sprite();

	if (getComponent<Bullet>()->guided) { s.setTexture(mTextures.get(Textures::Missile)); s.setScale(sf::Vector2f(1.2f, 1.45f)); }
	else { s.setTexture(mTextures.get(Textures::Bullet)); s.setScale(sf::Vector2f(0.5f, 0.5f)); }

	sf::FloatRect bounds = s.getLocalBounds();
	s.setOrigin(bounds.width / 2, bounds.height / 2);

	Sprite sprite = Sprite(s);
	addComponent<Sprite>(sprite);
}

void Projectile::update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs)
{
	ProjectileLogic::update(dt, entities, newEntities, pairs);
}

void Projectile::draw(sf::RenderTarget& target)
{
	//SpriteRenderer::render(this, target, *getComponent<Transform>());
}