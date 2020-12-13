#pragma once
#include "Component.h"

#include <SFML/Graphics/RectangleShape.hpp>

struct HealthBar : public IdentifiableComponent<HealthBar>
{
    HealthBar(
		const sf::Vector2f& size,
		const sf::Color& 	backgroundColor, 
		const sf::Color& 	fillColor, 
		const float 		fillRate = 0.f);

	sf::RectangleShape background;
	sf::RectangleShape bar;
};