#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Health.h"
#include "Common/Components/HealthBar.h"
#include "OldEntity.h"

class RenderHealthBar : public SignedSystem<Transform, Health, HealthBar>
{
public:
	static void render(OldEntity* entity, sf::RenderTarget& target);
};