#include "Common/Systems/TrajectoryRenderer.h"
#include "Common/Managers/GameManager.h"

void TrajectoryRenderer::update(const sf::Time&)
{
    for (Entity* entity: entities)
    {
        sf::View view = sf::View(sf::FloatRect(0, 0, 1600, 900));
		view.setViewport(sf::FloatRect(0, 0, 1, 1));
		view.setCenter(entity->getComponent<Transform>()->position);
		GameManager::getInstance()->getRenderTarget()->setView(view);

        GameManager::getInstance()->getRenderTarget()->draw(entity->getComponent<Trajectory>()->trajectory);
    }
}