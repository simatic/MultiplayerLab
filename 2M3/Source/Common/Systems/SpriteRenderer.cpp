#include "Common/Systems/SpriteRenderer.h"
#include "Common/Managers/GameManager.h"

SpriteRenderer::SpriteRenderer(GameManager* const gameManager) :
	RenderSystem<Transform, Sprite>(gameManager)
{}

void SpriteRenderer::update(const sf::Time&)
{
	for (Entity* entity: entities)
	{
		Sprite* s = entity->getComponent<Sprite>();
		s->sprite.setColor(s->colorFilter);
		s->sprite.setPosition(entity->getComponent<Transform>()->position);
		s->sprite.setRotation(entity->getComponent<Transform>()->rotation);
		gameManager->getRenderTarget()->draw(s->sprite);
	}
}
