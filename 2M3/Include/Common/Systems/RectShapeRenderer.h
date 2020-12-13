#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include "Common/Systems/System.h"
#include "../Components/RectShape.h"
#include "../Components/Transform.h"

class RectShapeRenderer : public RenderSystem<Transform, RectShape>
{
public:
	explicit RectShapeRenderer(GameManager* const gameManager);

	void update(const sf::Time& dt) override;
};