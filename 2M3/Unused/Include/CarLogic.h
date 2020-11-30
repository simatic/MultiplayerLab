#pragma once
#include "OldEntity.h"
#include <ProjectileLogic.h>
#include <math.h>
#include <KeyBinding.h>
#include <Particles.h>
#include <queue>
#include <functional>
#include "Common/Components/CarEngine.h"

class CarLogic : public OldEntity
{
public:
	CarLogic();
	CarLogic(int hp, sf::Vector2f pos, sf::RectangleShape rect);
	CarLogic(int hp, sf::Vector2f pos, sf::RectangleShape rect, KeyBinding* keys);

	virtual void update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs) override;

	virtual void instanciateBullet(const sf::Vector2f& position, const sf::Vector2f& direction, std::vector<OldEntity*>& newEntities);

protected:
	std::function<ProjectileLogic* (sf::Vector2f position, sf::Vector2f direction)> mInstanciateMissile;
	std::function<ProjectileLogic* (sf::Vector2f position, sf::Vector2f direction)> mInstanciateBullet;

private:
	KeyBinding* mKeyBindings;
};