#pragma once
#include <Entity.h>

// Voir note Car.h
class Wall : public Entity {

public:
    Wall(sf::Vector2f position, sf::RectangleShape shape, TextureHolder &textures);

    void onCollision(Entity *other) override;

    void update(sf::Time dt, std::vector<Entity *> entities, std::vector<Entity *> &newEntities,
                std::set<Pair> &pairs) override;
};
