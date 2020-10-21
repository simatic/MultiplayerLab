#pragma once
#include "../../Entity.h"

class Component
{
public:
	Component(Entity* entity);
	virtual ~Component() = default;

private:
	Entity* entity;
};