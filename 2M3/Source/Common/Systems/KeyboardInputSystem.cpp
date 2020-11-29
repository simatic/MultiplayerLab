#include "Common/Systems/KeyboardInputSystem.h"
#include "Common/Managers/GameManager.h"

void KeyboardInputSystem::update(const sf::Time&)
{
	for (Entity* entity: entities)
	{
		CarInput* inputs = entity->getComponent<CarInput>();
		
		if (GameManager::getInstance()->getKeyBinding() != nullptr)
		{
			inputs->left = sf::Keyboard::isKeyPressed(GameManager::getInstance()->getKeyBinding()->getAssignedKey(PlayerAction::TurnLeft));
			inputs->right = sf::Keyboard::isKeyPressed(GameManager::getInstance()->getKeyBinding()->getAssignedKey(PlayerAction::TurnRight));
			inputs->up = sf::Keyboard::isKeyPressed(GameManager::getInstance()->getKeyBinding()->getAssignedKey(PlayerAction::Accelerate));
			inputs->down = sf::Keyboard::isKeyPressed(GameManager::getInstance()->getKeyBinding()->getAssignedKey(PlayerAction::Brake));
			inputs->action = sf::Keyboard::isKeyPressed(GameManager::getInstance()->getKeyBinding()->getAssignedKey(PlayerAction::DoAction));
		}
	}
}