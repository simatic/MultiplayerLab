#pragma once
#include <OldEntity.h>
#include "Common/Components/Bullet.h"

class CarLogic; // CarLogic forward declaration

class ProjectileLogic : public OldEntity
{
public:
	ProjectileLogic(int dmg, sf::Time lifetime, float speed, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, CarLogic* car);
	ProjectileLogic(int dmg, sf::Time lifetime, float speed, float detection, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, CarLogic* car);

	virtual void update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs) override;
	void onCollision(OldEntity* other) override;

	int getDamage();
	CarLogic* getCar();
	bool isGuided();

protected:
	Bullet bullet;

	bool mGuided;
	OldEntity* mTarget;
	float mDetectionRange;
	float mGuideRate;

	CarLogic* mCar;
};