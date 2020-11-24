#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Trajectory.h"
#include "Car.h"

class RenderTrajectorySystem : public SignedSystem<Transform, Trajectory>
{
public:
	static void render(Car* entity, sf::RenderTarget& target);
};