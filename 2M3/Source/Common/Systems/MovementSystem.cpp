#include "Common/Systems/MovementSystem.h"

void MovementSystem::update(const sf::Time& dt)
{
	for (Entity* entity: entities)
	{
		entity->getComponent<Transform>()->position += entity->getComponent<Kinematics>()->velocity * dt.asSeconds();
	}
}