#include "Common/Systems/SpriteRenderer.h"
#include "Common/Managers/GameManager.h"
#include <iostream>

void SpriteRenderer::update(const sf::Time&)
{
	for (Entity* entity: entities)
	{
		Sprite* s = entity->getComponent<Sprite>();
		s->sprite.setPosition(entity->getComponent<Transform>()->position);
		s->sprite.setRotation(entity->getComponent<Transform>()->rotation);
		GameManager::getInstance()->getRenderTarget()->draw(s->sprite);
	}
}