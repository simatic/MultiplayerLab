#pragma once
#include "Common/Components/Component.h"

#include <SFML/System/Vector2.hpp>

struct Transform : public IdentifiableComponent<Transform>
{
	Transform(const sf::Vector2f& position, const float rotation);
	std::ostream& serialize(std::ostream& os) const override;
	std::istream& deserialize(std::istream& is) override;

	sf::Vector2f 	position;
	float 			rotation;
};