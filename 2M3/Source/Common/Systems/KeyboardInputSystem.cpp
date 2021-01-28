#include "Common/Systems/KeyboardInputSystem.h"
#include "Common/Managers/GameManager.h"

KeyboardInputSystem::KeyboardInputSystem(GameManager* const gameManager) :
	LogicSystem<PlayableCarInput>(gameManager)
{}

void KeyboardInputSystem::update(const sf::Time&)
{
	for (Entity* entity: entities)
	{
        VehicleInput* inputs = entity->getComponent<PlayableCarInput>()->inputs;
		
		if (gameManager->getKeyBinding() != nullptr)
		{
			inputs->left = sf::Keyboard::isKeyPressed(gameManager->getKeyBinding()->getAssignedKey(PlayerAction::TurnLeft));
			inputs->right = sf::Keyboard::isKeyPressed(gameManager->getKeyBinding()->getAssignedKey(PlayerAction::TurnRight));
			inputs->up = sf::Keyboard::isKeyPressed(gameManager->getKeyBinding()->getAssignedKey(PlayerAction::Accelerate));
			inputs->down = sf::Keyboard::isKeyPressed(gameManager->getKeyBinding()->getAssignedKey(PlayerAction::Brake));
			inputs->action = sf::Keyboard::isKeyPressed(gameManager->getKeyBinding()->getAssignedKey(PlayerAction::DoAction));
		}
	}
}