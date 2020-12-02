#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Health.h"
#include "Common/Components/HealthBar.h"

class HealthBarRenderer : public RenderSystem<Transform, Health, HealthBar>
{
public:
	void update(const sf::Time& dt) override;;
};