#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/CarInput.h"

class KeyboardInputSystem : public LogicSystem<CarInput>
{
public:
	void update(const sf::Time& dt) override;
};