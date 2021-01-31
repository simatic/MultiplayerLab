#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include "Common/Systems/System.h"
#include "../Components/Sprite.h"
#include "../Components/Transform.h"
#include "Common/Entity.h"

/**
 * @class ParticleRenderer
 * Renders a sprite.
 */
class SpriteRenderer : public RenderSystem<Transform, Sprite>
{
public:
	explicit SpriteRenderer(GameManager* const gameManager);

	void update(const sf::Time& dt) override;
};