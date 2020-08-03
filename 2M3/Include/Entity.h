#pragma once

#include <SFML/System.hpp>

class Entity
{
public:
	explicit Entity(int hp);
	void setVelocity(sf::Vector2f velocity);
	sf::Vector2f getVelocity();
	void setAngle(float angle);
	void rotate(float angle);
	float getAngle();
	virtual void damage(int points);
	virtual void repair(int points);
	int getHp();
	void setHp(int hp);

private:
	sf::Vector2f mVelocity;
	float mAngle;
	int mHp;

};
