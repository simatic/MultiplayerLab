#pragma once
#include "Component.h"

/**
 * @struct Health component.
 * Holds information about an entity's health.
 */
struct Health : public IdentifiableComponent<Health>
{
    Health(const int health, const int maxHealth);

	int health;     //!< Current health.
    int maxHealth;  //!< Maximum health.
};