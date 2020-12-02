#include "Common/Systems/TrajectoryRenderer.h"
#include "Common/Managers/GameManager.h"

void TrajectoryRenderer::update(const sf::Time&)
{
    for (Entity* entity: entities)
    {
        GameManager::getInstance()->getRenderTarget()->draw(entity->getComponent<Trajectory>()->trajectory);
    }
}