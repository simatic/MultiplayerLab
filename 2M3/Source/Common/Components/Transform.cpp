#include "Common/Components/Transform.h"

Transform::Transform(const sf::Vector2f& position, const float rotation) :
	position(position),
	rotation(rotation)
{}

std::ostream& Transform::serialize(std::ostream& os) const {
	return os << position.x << position.y << rotation;
}

std::istream& Transform::deserialize(std::istream& is) {
	return is >> position.x >> position.y >> rotation;
}
