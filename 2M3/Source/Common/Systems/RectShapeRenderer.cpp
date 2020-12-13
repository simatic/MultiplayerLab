#include "Common/Systems/RectShapeRenderer.h"
#include "Common/Managers/GameManager.h"

RectShapeRenderer::RectShapeRenderer(GameManager* const gameManager) :
	RenderSystem<Transform, RectShape>(gameManager)
{}

void RectShapeRenderer::update(const sf::Time&)
{
	for (Entity* entity: entities)
	{
		RectShape* rect = entity->getComponent<RectShape>();
		rect->shape.setPosition(entity->getComponent<Transform>()->position);
		rect->shape.setRotation(entity->getComponent<Transform>()->rotation);
		gameManager->getRenderTarget()->draw(rect->shape);
	}
}