#include <SFML/System.hpp>
#include <math.h>

#include "Entity.h"

Entity::Entity(int hp)
{
	mHp = hp;
}

void Entity::setVelocity(sf::Vector2f velocity) 
{
	mVelocity = velocity;
}

sf::Vector2f Entity::getVelocity()
{
	return mVelocity;
}

void Entity::setAngle(float angle)
{
	mAngle = angle;
}

float Entity::getAngle()
{
	return mAngle;
}

void Entity::rotate(float angle)
{
	mAngle += angle;
	if (mAngle < 0) 
	{
		mAngle = fmod(mAngle, 360);
		mAngle = 360 - mAngle;
	}
	else if(mAngle >= 360)
	{
		mAngle = fmod(mAngle, 360);
	}
}

void Entity::damage(int points)
{
	mHp -= points;
}

void Entity::repair(int points)
{
	mHp += points;
}

int Entity::getHp()
{
	return mHp;
}

void Entity::setHp(int hp)
{
	mHp = hp;
}
