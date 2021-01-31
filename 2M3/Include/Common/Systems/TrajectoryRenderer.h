#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Trajectory.h"

/**
 * @class TrajectoryRenderer
 * Renders the trajectory of the vehicle.
 */
class TrajectoryRenderer : public RenderSystem<Transform, Trajectory>
{
public:
	explicit TrajectoryRenderer(GameManager* const gameManager);

	void update(const sf::Time& dt) override;;
};