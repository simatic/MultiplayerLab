#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Health.h"
#include "Common/Components/HealthBar.h"

/**
 * @class HealthBarRenderer
 * Renders a health bar below vehicles.
 */
class HealthBarRenderer : public RenderSystem<Transform, Health, HealthBar>
{
public:
	explicit HealthBarRenderer(GameManager* const gameManager);

	void update(const sf::Time& dt) override;;
};