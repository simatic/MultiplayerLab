#pragma once
#include <OldEntity.h>

class WallLogic : public OldEntity {

public:
    WallLogic(sf::Vector2f position, sf::RectangleShape colliderShape);

    void update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities,
        std::set<Pair>& pairs) override;
};