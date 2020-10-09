//
// Created by jglrxavpok on 09/10/2020.
//

#include "Wall.h"

Wall::Wall(sf::Vector2f position, sf::RectangleShape shape, TextureHolder& textures):
    Entity(position, std::move(shape), textures) {
    mShape.setFillColor(sf::Color::Blue);
    mShape.setPosition(mPosition+mShape.getSize()/2.0f);
}

// don't do anything on collision
void Wall::onCollision(Entity *other) {}

void Wall::update(sf::Time dt, std::vector<Entity *> entities, std::vector<Entity *> &newEntities, std::set<Pair> &pairs) {}
