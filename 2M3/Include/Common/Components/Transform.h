#pragma once
#include <SFML/System/Vector2.hpp>
#include "Common/Components/Component.h"

struct Transform : Component
{
	sf::Vector2f position;
	float rotation;

	Transform(sf::Vector2f position, float rotation);
};