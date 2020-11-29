#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <set>
#include "Common/Components/Component.h"
#include "Common/Entity.h"

struct Collider : public IdentifiableComponent<Collider>
{
	Collider(const sf::Vector2f& size);
	Collider(const sf::RectangleShape& shape);

	sf::RectangleShape	shape;
	bool				collides = false;
	std::set<Entity*>others;
};