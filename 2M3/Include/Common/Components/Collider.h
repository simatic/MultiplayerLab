#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include "Common/Components/Component.h"

struct Collider : public IdentifiableComponent<Collider>
{
	sf::RectangleShape	shape;
	bool				collides = false;

	Collider(const sf::Vector2f& size);
	Collider(const sf::RectangleShape& shape);
};