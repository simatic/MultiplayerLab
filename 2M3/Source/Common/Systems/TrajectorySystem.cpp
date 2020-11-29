#include "Common/Systems/TrajectorySystem.h"

void TrajectorySystem::update(const sf::Time&)
{
    for (Entity* entity: entities)
    {   
        Transform* transform = entity->getComponent<Transform>();
        Trajectory* trajectory = entity->getComponent<Trajectory>();

        trajectory->trajectory.append(sf::Vertex(transform->position - (float)20 * entity->getComponent<CarEngine>()->direction));
        trajectory->trajectory.append(sf::Vertex(transform->position - (float)20 * entity->getComponent<CarEngine>()->direction));
    }
}