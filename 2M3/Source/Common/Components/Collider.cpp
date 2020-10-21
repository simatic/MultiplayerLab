#include "Common/Components/Collider.h"

Collider::Collider(const sf::Vector2f& size) :
	shape(size)
{}

Collider::Collider(const sf::RectangleShape& shape) :
	shape(shape)
{}