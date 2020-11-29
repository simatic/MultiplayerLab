#include "Common/Systems/RenderSystem.h"
#include "Common/Managers/GameManager.h"
#include <iostream>

void RenderSystem::update(const sf::Time&)
{
	for (Entity* entity: entities)
	{
		sf::View view = sf::View(sf::FloatRect(0, 0, 1600, 900));
		view.setViewport(sf::FloatRect(0, 0, 1, 1));
		view.setCenter(entity->getComponent<Transform>()->position);
		GameManager::getInstance()->getRenderTarget()->setView(view);

		Sprite* s = entity->getComponent<Sprite>();
		s->sprite.setPosition(entity->getComponent<Transform>()->position);
		s->sprite.setRotation(entity->getComponent<Transform>()->rotation);
		GameManager::getInstance()->getRenderTarget()->draw(s->sprite);
	}
}