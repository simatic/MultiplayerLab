#include "Entity.h"
#include "Car.h"

#include <SFML/System.hpp>

void Car::damage(int points) 
{
	if (getHitpoints() - points <= 0)
	{
		setHitpoints(0);
	}
	else
	{
		setHitpoints(getHitpoints() + points);
	}
}

void Car::repair(int points)
{
	if (getHitpoints() + points >= mHpMax)
	{
		setHitpoints(mHpMax);
	}
	else
	{
		setHitpoints(getHitpoints() + points);
	}
}