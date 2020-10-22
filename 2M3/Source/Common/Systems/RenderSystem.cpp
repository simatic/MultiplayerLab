#include "Common/Systems/RenderSystem.h"

void RenderSystem::render(sf::RenderTarget& target, Transform transform, Sprite sprite)
{
	sprite.sprite.setPosition(transform.position);
	target.draw(sprite.sprite);
}