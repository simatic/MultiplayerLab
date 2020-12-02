#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/CarEngine.h"
#include "Common/Components/Trajectory.h"

class TrajectorySystem : public LogicSystem<Transform, CarEngine, Trajectory>
{
public:
	void update(const sf::Time& dt) override;
};