#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/VehicleInput.h"
#include "Common/Components/VehicleEngine.h"

class CarMovementSystem : public LogicSystem<Transform, Kinematics, VehicleInput, VehicleEngine>
{
public:
	explicit CarMovementSystem(GameManager* const gameManager);

	void update(const sf::Time& dt) override;;
};