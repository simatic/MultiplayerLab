#include "Common/Systems/MovementSystem.h"

MovementSystem::MovementSystem(GameManager* const gameManager) :
	LogicSystem<Transform, Kinematics>(gameManager)
{}

void MovementSystem::update(const sf::Time& dt)
{
	// TODO: Update position
}