#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/VehicleEngine.h"
#include "Common/Components/Trajectory.h"

class TrajectorySystem : public LogicSystem<Transform, VehicleEngine, Trajectory>
{
public:
	explicit TrajectorySystem(GameManager* const gameManager);

	void update(const sf::Time& dt) override;
};