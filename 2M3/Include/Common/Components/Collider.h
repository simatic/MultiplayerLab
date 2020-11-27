#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <set>
#include "Common/Components/Component.h"
#include "OldEntity.h"

class OldEntity;

struct Collider : public IdentifiableComponent<Collider>
{
	Collider(const sf::Vector2f& size);
	Collider(const sf::RectangleShape& shape);

	sf::RectangleShape	shape;
	bool				collides = false;
	std::set<OldEntity*>others;
};