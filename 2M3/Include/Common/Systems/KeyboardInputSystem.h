#pragma once

#include <PlayableVehicleInput.h>
#include "Common/Systems/System.h"
#include "Common/Components/VehicleInput.h"

class KeyboardInputSystem : public LogicSystem<PlayableVehicleInput>
{
public:
	explicit KeyboardInputSystem(GameManager* const gameManager);

	void update(const sf::Time& dt) override;
};