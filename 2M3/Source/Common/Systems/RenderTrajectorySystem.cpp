#include "Common/Systems/RenderTrajectorySystem.h"

void RenderTrajectorySystem::render(Car* entity, sf::RenderTarget& target)
{
    target.draw(entity->getComponent<Trajectory>()->trajectory);
}