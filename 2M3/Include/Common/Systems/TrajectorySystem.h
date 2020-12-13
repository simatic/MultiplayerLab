#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/CarEngine.h"
#include "Common/Components/Trajectory.h"

class TrajectorySystem : public LogicSystem<Transform, CarEngine, Trajectory>
{
public:
	explicit TrajectorySystem(GameManager* const gameManager);

	void update(const sf::Time& dt) override;
};