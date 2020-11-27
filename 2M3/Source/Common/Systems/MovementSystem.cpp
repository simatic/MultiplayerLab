#include "Common/Systems/MovementSystem.h"

void MovementSystem::update(const sf::Time dt, OldEntity* entity)
{
	entity->getComponent<Transform>()->position += entity->getVelocity() * dt.asSeconds();
}