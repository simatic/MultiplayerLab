#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Collider.h"

class CollisionSystem : public SignedSystem<Transform, Collider>
{
public:
	void update(const sf::Time& dt) override;;
};