#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/CarInput.h"
#include "Common/Components/VehicleEngine.h"

class CarMovementSystem : public LogicSystem<Transform, Kinematics, CarInput, VehicleEngine>
{
public:
	explicit CarMovementSystem(GameManager* const gameManager);

	void update(const sf::Time& dt) override;;
};