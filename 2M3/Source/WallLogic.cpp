#include "WallLogic.h"

WallLogic::WallLogic(sf::Vector2f position, sf::RectangleShape colliderShape) :
    Entity(position, std::move(colliderShape)) {
    mCollider.shape.setPosition(mTransform.position + mCollider.shape.getSize() / 2.0f);
}

// don't do anything on collision
void WallLogic::onCollision(Entity* other) {}

void WallLogic::update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs) {}