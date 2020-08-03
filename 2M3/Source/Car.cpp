#include "Entity.h"
#include "Car.h"

#include <SFML/System.hpp>

void Car::damage(int points) 
{
	if (getHp() - points <= 0)
	{
		setHp(0);
	}
	else
	{
		setHp(getHp()+points);
	}
}

void Car::repair(int points)
{
	if (getHp() + points >= mHpMax)
	{
		setHp(mHpMax);
	}
	else
	{
		setHp(getHp()+points);
	}
}