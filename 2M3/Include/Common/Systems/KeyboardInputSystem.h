#pragma once

#include <PlayableCarInput.h>
#include "Common/Systems/System.h"
#include "Common/Components/CarInput.h"

class KeyboardInputSystem : public LogicSystem<PlayableCarInput>
{
public:
	explicit KeyboardInputSystem(GameManager* const gameManager);

	void update(const sf::Time& dt) override;
};