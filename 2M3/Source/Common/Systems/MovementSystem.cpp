#include "Common/Systems/MovementSystem.h"

MovementSystem::MovementSystem(GameManager* const gameManager) :
	LogicSystem<Transform, Kinematics>(gameManager)
{}

void MovementSystem::update(const sf::Time& dt)
{
	for (Entity* entity: entities)
	{
		entity->getComponent<Transform>()->position += entity->getComponent<Kinematics>()->velocity * dt.asSeconds();
	}
}