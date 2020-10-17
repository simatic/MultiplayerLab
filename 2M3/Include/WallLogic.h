#pragma once
#include <Entity.h>

class WallLogic : public Entity {

public:
    WallLogic(sf::Vector2f position, sf::RectangleShape colliderShape);

    void onCollision(Entity* other) override;

    void update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities,
        std::set<Pair>& pairs) override;
};