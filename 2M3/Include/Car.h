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
	void serverUpdate(sf::Time serverTime, sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs) override;

	void useInputs(sf::Time dt, std::vector<OldEntity*>& newEntities) override;

	void draw(sf::RenderTarget& target) override;

	std::string getActionText();
	bool getShowMap();

	void setSprite();

protected:
	void instanciateBullet(const sf::Vector2f& position, const sf::Vector2f& direction, std::vector<OldEntity*>& newEntities) override;

private:
	const TextureHolder& mTextures;

	sf::RectangleShape mHpBackgroundBar;
	sf::RectangleShape mHpBar;

	bool mShowMap;

	std::queue<SpatialVars> mTrajectory;
};