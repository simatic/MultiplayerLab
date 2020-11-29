#include "Common/Systems/RenderTrajectorySystem.h"

void RenderTrajectorySystem::update(const sf::Time&)
{
    for (Entity* entity: entities)
    {
        //target.draw(entity->getComponent<Trajectory>()->trajectory);
    }
}