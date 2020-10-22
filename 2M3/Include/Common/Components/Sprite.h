#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "Component.h"

struct Sprite
{
	sf::Sprite sprite;

	Sprite(const sf::Sprite& sprite);
};