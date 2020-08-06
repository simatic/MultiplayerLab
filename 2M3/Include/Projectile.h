#pragma once
#include <Entity.h>

class Projectile : public Entity
{
public:
	Projectile(int dmg, sf::Vector2f pos, sf::Vector2f velocity, sf::RectangleShape rect);


private:
	int mDamage;

};