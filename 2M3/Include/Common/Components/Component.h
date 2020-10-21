#pragma once
#include <SFML/System/Time.hpp>
#include "../../Entity.h"

/*
* \class Component
* \brief Base class for all components.
*/
class Component
{
public:
	Component(Entity* entity);
	virtual ~Component() = default;

	virtual void update(sf::Time dt);

private:
	Entity* entity;
};