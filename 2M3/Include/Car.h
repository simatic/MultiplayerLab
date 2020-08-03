#pragma once
#include <SFML/System.hpp>

#include "Entity.h"

class Car : public Entity
{
public:
	void damage(int points) override;
	void repair(int points) override;

private:
	int mHpMax;

};