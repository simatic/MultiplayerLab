#include "Common/Systems/CarDeath.h"
#include "Common/Managers/GameManager.h"

CarDeath::CarDeath(GameManager* const gameManager) :
    LogicSystem<Health>(gameManager)
{}

void CarDeath::update(const sf::Time& dt)
{
    for (Entity* entity: entities)
    {
        Health* health = entity->getComponent<Health>();

        if (health->health <= 0.f)
        {
            gameManager->removeEntityNextFrame(entity);
        }
    }
}