#pragma once
#include "Component.h"

#include <SFML/Graphics/Sprite.hpp>

struct Sprite : public IdentifiableComponent<Sprite>
{
	Sprite(const sf::Sprite& sprite);

	sf::Sprite sprite;

    sf::Color colorFilter{255, 255, 255, 255};
};