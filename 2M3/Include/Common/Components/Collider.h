#pragma once
#include "Common/Components/Component.h"
#include "Common/Entity.h"

#include <set>
#include <SFML/Graphics/RectangleShape.hpp>

struct Collider : public IdentifiableComponent<Collider>
{
	Collider(const sf::Vector2f& size);
	Collider(const sf::RectangleShape& shape);

	bool				collides = false;
	sf::RectangleShape	shape;
	std::set<Entity*>	others;
};