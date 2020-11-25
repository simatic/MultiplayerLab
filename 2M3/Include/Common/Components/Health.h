#pragma once
#include "Component.h"

struct Health : public IdentifiableComponent<Health>
{
    Health(const int health, const int maxHealth);

	int health;
    int maxHealth;
};