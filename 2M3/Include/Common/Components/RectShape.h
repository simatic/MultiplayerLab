#pragma once
#include "Common/Components/Component.h"

#include <SFML/Graphics/RectangleShape.hpp>

/**
 * @struct RectShape component.
 * A rectangular shape.
 */
struct RectShape : public IdentifiableComponent<RectShape>
{
    RectShape() = default;

    sf::RectangleShape shape;
};