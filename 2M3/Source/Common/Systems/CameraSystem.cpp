#include <SFML/Graphics.hpp>
#include <Common/Managers/GameManager.h>
#include "CameraSystem.h"

CameraSystem::CameraSystem() {}

void CameraSystem::update(const sf::Time& dt) {
    if(entities.empty())
        return;
    auto* entity = *entities.begin();
    sf::View view = sf::View(sf::FloatRect(0, 0, 1600, 900));
    view.setViewport(sf::FloatRect(0, 0, 1, 1));
    view.setCenter(entity->getComponent<Transform>()->position);
    GameManager::getInstance()->getRenderTarget()->setView(view);
}
