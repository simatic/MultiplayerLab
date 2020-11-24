#include "Common/Systems/TrajectorySystem.h"

void TrajectorySystem::update(Car* entity)
{
    Transform* transform = entity->getComponent<Transform>();
    Trajectory* trajectory = entity->getComponent<Trajectory>();

    trajectory->trajectory.append(sf::Vertex(transform->position - (float)20 * entity->getCarDirection()));
    trajectory->trajectory.append(sf::Vertex(transform->position - (float)20 * entity->getCarDirection()));
}