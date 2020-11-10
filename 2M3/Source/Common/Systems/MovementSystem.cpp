#include "Common/Systems/MovementSystem.h"

void MovementSystem::update(const sf::Time dt, OldEntity* entity)
{
	entity->getPosition() += entity->getVelocity() * dt.asSeconds();
}