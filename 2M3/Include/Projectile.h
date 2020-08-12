#pragma once
#include <Entity.h>
#include <Car.h>

class Projectile : public Entity
{
public:
	Projectile(int dmg, sf::Vector2f pos, sf::Vector2f velocity, sf::RectangleShape rect, Car* car, sf::Time lifetime);

	void update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs) override;
	void onCollision(Entity* other) override;

	int getDamage();
	Car* getCar();

private:
	int mDamage;
	sf::Time mLifetime;
	Car* mCar;
};