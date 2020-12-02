#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Kinematics.h"

class MovementSystem : public LogicSystem<Transform, Kinematics>
{
public:
	void update(const sf::Time& dt) override;;
};