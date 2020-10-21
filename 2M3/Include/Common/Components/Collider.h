#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include "Common/Components/Component.h"

struct Collider
{
	sf::RectangleShape shape;

	Collider(const sf::Vector2f& size);
	Collider(const sf::RectangleShape& shape);
};