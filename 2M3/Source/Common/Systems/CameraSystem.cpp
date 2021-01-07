#include <SFML/Graphics.hpp>
#include <Common/Managers/GameManager.h>
#include "CameraSystem.h"

CameraSystem::CameraSystem(GameManager* const gameManager) :
    RenderSystem<Transform, CameraTarget>(gameManager)
{}

void CameraSystem::update(const sf::Time& dt) {
    if(entities.empty())
        return;
    auto* entity = *entities.begin();
    float w = gameManager->getRenderTarget()->getSize().x;
    float h = gameManager->getRenderTarget()->getSize().y;
    sf::View view = sf::View(sf::FloatRect(0, 0, w, h));
    view.setViewport(sf::FloatRect(0, 0, 1, 1));
    view.setCenter(entity->getComponent<Transform>()->position);
    gameManager->getRenderTarget()->setView(view);
}
