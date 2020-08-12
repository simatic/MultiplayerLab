#pragma once
#include <Entity.h>
#include <Car.h>

class Projectile : public Entity
{
public:
	Projectile(int dmg, sf::Vector2f pos, sf::Vector2f velocity, sf::RectangleShape rect, Car* car);

	void onCollision(Entity* other) override;

	int getDamage();
	Car* getCar();

private:
	int mDamage;

	Car* mCar;
};