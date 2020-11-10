#include "WallLogic.h"

WallLogic::WallLogic(sf::Vector2f position, sf::RectangleShape colliderShape) :
    OldEntity(position, std::move(colliderShape)) {
    mCollider.shape.setPosition(mTransform.position + mCollider.shape.getSize() / 2.0f);
}

// don't do anything on collision
void WallLogic::onCollision(OldEntity* other) {}

void WallLogic::update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs) {}