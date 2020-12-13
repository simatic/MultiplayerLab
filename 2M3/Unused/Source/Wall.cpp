//
// Created by jglrxavpok on 09/10/2020.
//

#include "Wall.h"

Wall::Wall(sf::Vector2f position, sf::RectangleShape colliderShape, sf::RectangleShape renderedShape) :
    WallLogic(position, std::move(colliderShape)),
    mRenderedShape(renderedShape)
{
    mRenderedShape.setPosition(position);
    mRenderedShape.setFillColor(sf::Color::Blue);
}

void Wall::draw(sf::RenderTarget& target)
{
    target.draw(mRenderedShape);
}
