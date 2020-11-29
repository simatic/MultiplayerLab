#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Kinematics.h"

class MovementSystem : public SignedSystem<Transform, Kinematics>
{
public:
	void update(const sf::Time& dt) override;;
};