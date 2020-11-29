#include "Common/Systems/RenderTrajectorySystem.h"
#include "Common/Managers/GameManager.h"

void RenderTrajectorySystem::update(const sf::Time&)
{
    for (Entity* entity: entities)
    {
        GameManager::getInstance()->getRenderTarget()->draw(entity->getComponent<Trajectory>()->trajectory);
    }
}