#include "Common/Systems/MovementSystem.h"
#include "Common/Components/Kinematics.h"

void MovementSystem::update(const sf::Time dt, OldEntity* entity)
{
	entity->getComponent<Transform>()->position += entity->getComponent<Kinematics>()->velocity * dt.asSeconds();
}