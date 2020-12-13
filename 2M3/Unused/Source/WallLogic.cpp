#include "WallLogic.h"

WallLogic::WallLogic(sf::Vector2f position, sf::RectangleShape colliderShape) :
    OldEntity(position, std::move(colliderShape)) {
    getComponent<Collider>()->shape.setPosition(getComponent<Transform>()->position + getComponent<Collider>()->shape.getSize() / 2.0f);
}

void WallLogic::update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs) {}