#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include "Component.h"

struct HealthBar : public IdentifiableComponent<HealthBar>
{
    HealthBar(const sf::Vector2f& origin, const sf::Vector2f& size, const sf::Color& backgroundColor, const sf::Color& fillColor, const float fillRate);

	sf::RectangleShape background;
	sf::RectangleShape bar;
};