#include "Common/Systems/CarDeath.h"
#include "Common/Managers/GameManager.h"

void CarDeath::update(const sf::Time& dt)
{
    for (Entity* entity: entities)
    {
        Health* health = entity->getComponent<Health>();

        if (health->health <= 0.f)
        {
            GameManager::getInstance()->removeEntityNextFrame(entity->getID());
        }
    }
}