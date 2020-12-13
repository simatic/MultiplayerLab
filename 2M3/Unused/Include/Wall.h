#pragma once
#include "Renderable.h"
#include "WallLogic.h"

class Wall : public WallLogic, public Renderable {

public:
    Wall(sf::Vector2f position, sf::RectangleShape colliderShape, sf::RectangleShape renderedShape);

    void draw(sf::RenderTarget& target) override;

private:
    sf::RectangleShape mRenderedShape;
};
