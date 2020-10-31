#pragma once
#include <SFML/System/Vector2.hpp>
#include "Common/Components/Component.h"

struct Transform : public IdentifiableComponent<Transform>
{
	sf::Vector2f position;
	float rotation;

	Transform(const sf::Vector2f& position, const float rotation);
};