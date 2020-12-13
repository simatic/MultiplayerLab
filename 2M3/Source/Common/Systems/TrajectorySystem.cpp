#include "Common/Systems/TrajectorySystem.h"

TrajectorySystem::TrajectorySystem(GameManager* const gameManager) :
    LogicSystem<Transform, CarEngine, Trajectory>(gameManager)
{}

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