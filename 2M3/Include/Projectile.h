#pragma once
#include <Entity.h>
#include <Car.h>

class Projectile : public Entity
{
public:
	Projectile(int dmg, sf::Time lifetime, float speed, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, Car* car, const TextureHolder& textures);
	Projectile(int dmg, sf::Time lifetime, float speed, float detection, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect, Car* car, const TextureHolder& textures);

	void update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs) override;
	void onCollision(Entity* other) override;

	void setSprite() override;

	int getDamage();
	Car* getCar();
	bool isGuided();

private:
	int mDamage;
	float mMaxSpeed;
	sf::Time mLifetime;

	bool mGuided;
	Entity* mTarget;
	float mDetectionRange;
	float mGuideRate;

	Car* mCar;
};