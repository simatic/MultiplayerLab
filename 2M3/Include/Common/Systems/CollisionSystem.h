#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Collider.h"

class CollisionSystem : public LogicSystem<Transform, Collider>
{
public:
	void update(const sf::Time& dt) override;;
};