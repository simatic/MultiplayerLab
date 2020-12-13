#pragma once
#define _USE_MATH_DEFINES
#include "CarLogic.h"
#include "Renderable.h"
#include <math.h>
#include <KeyBinding.h>
#include <queue>

class Car : public CarLogic, public Renderable
{
public:
	Car(const TextureHolder& textures);
	Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, const TextureHolder& textures);
	Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, KeyBinding* keys, const TextureHolder& textures);

	void update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs) override;
	void draw(sf::RenderTarget& target) override;

	void setSprite();

protected:
	void instanciateBullet(const sf::Vector2f& position, const sf::Vector2f& direction, std::vector<OldEntity*>& newEntities) override;

private:
	const TextureHolder& mTextures;
};