#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Trajectory.h"
#include "Car.h"

class TrajectorySystem : public SignedSystem<Trajectory>
{
public:
	static void update(Car* entity);
};