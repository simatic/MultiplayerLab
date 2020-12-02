#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Trajectory.h"

class TrajectoryRenderer : public RenderSystem<Transform, Trajectory>
{
public:
	void update(const sf::Time& dt) override;;
};