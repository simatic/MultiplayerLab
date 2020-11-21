#include "Common/Systems/RenderSystem.h"

void RenderSystem::render(OldEntity* entity, sf::RenderTarget& target, Transform transform)
{	
	Sprite* sprite = entity->getComponent<Sprite>();
	sprite->sprite.setPosition(transform.position);
	sprite->sprite.setRotation(transform.rotation);
	target.draw(sprite->sprite);
}