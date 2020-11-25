#include "Common/Systems/KeyboardInputSystem.h"

void KeyboardInputSystem::update(OldEntity* entity, KeyBinding* keyBindings)
{
    CarInput* inputs = entity->getComponent<CarInput>();

    if (keyBindings != nullptr)
	{
		inputs->left = sf::Keyboard::isKeyPressed(keyBindings->getAssignedKey(PlayerAction::TurnLeft));
		inputs->right = sf::Keyboard::isKeyPressed(keyBindings->getAssignedKey(PlayerAction::TurnRight));
		inputs->up = sf::Keyboard::isKeyPressed(keyBindings->getAssignedKey(PlayerAction::Accelerate));
		inputs->down = sf::Keyboard::isKeyPressed(keyBindings->getAssignedKey(PlayerAction::Brake));
		inputs->action = sf::Keyboard::isKeyPressed(keyBindings->getAssignedKey(PlayerAction::DoAction));
	}
}