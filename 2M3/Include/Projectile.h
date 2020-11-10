#pragma once
#include "Renderable.h"
#include "Car.h"
#include "ProjectileLogic.h"
#include "ResourceIdentifiers.h"

class Projectile : public ProjectileLogic, public Renderable
{
public:
	Projectile(int dmg, sf::Time lifetime, float speed, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, Car* car, const TextureHolder& textures);
	Projectile(int dmg, sf::Time lifetime, float speed, float detection, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, Car* car, const TextureHolder& textures);

	void update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs) override;

	void setSprite();

	void draw(sf::RenderTarget& target) override;

private:
	const TextureHolder&	mTextures;

	sf::Sprite				mSprite;
};