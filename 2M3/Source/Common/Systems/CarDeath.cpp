#include "Common/Systems/CarDeath.h"

void CarDeath::update(OldEntity* entity)
{
    Health* health = entity->getComponent<Health>();

    if (health->health <= 0.f)
    {
        entity->setToRemove(true);
    }
}