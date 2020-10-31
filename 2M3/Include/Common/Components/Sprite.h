#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "Component.h"

struct Sprite : public IdentifiableComponent<Sprite>
{
	sf::Sprite sprite;

	Sprite(const sf::Sprite& sprite);
};