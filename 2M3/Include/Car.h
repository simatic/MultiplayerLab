#pragma once
#define _USE_MATH_DEFINES
#include <CarLogic.h>
#include "Renderable.h"
#include <math.h>
#include <KeyBinding.h>
#include <Particles.h>
#include <queue>

class Car : public CarLogic, public Renderable
{
public:
	Car(const TextureHolder& textures);
	Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, const TextureHolder& textures);
	Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, KeyBinding* keys, const TextureHolder& textures);

	void update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs) override;
	void serverUpdate(sf::Time serverTime, sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs) override;

	void useInputs(sf::Time dt, std::vector<Entity*>& newEntities) override;

	void draw(sf::RenderTarget& target) override;

	std::string getActionText();
	bool getShowMap();

	void setSprite();

protected:
	void instanciateBullet(const sf::Vector2f& position, const sf::Vector2f& direction, std::vector<Entity*>& newEntities) override;

private:
	const TextureHolder& mTextures;
	sf::Sprite mSprite;

	sf::RectangleShape mHpBackgroundBar;
	sf::RectangleShape mHpBar;

	sf::VertexArray mTires;
	Particles mDust;

	bool mShowMap;

	std::queue<SpatialVars> mTrajectory;
};