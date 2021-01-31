#include "Common/Systems/VehicleDeath.h"
#include "Common/Managers/GameManager.h"

VehicleDeath::VehicleDeath(GameManager* const gameManager) :
    LogicSystem<Health>(gameManager)
{}

void VehicleDeath::update(const sf::Time& dt)
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