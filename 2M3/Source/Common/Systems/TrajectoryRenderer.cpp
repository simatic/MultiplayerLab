#include "Common/Systems/TrajectoryRenderer.h"
#include "Common/Managers/GameManager.h"

TrajectoryRenderer::TrajectoryRenderer(GameManager* const gameManager) :
    RenderSystem<Transform, Trajectory>(gameManager)
{}

void TrajectoryRenderer::update(const sf::Time&)
{
    for (Entity* entity: entities)
    {
        gameManager->getRenderTarget()->draw(entity->getComponent<Trajectory>()->trajectory);
    }
}