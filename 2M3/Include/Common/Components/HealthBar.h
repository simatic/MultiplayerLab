#pragma once
#include <Common/Components/Component.h>

#include <SFML/Graphics/RectangleShape.hpp>

/**
 * @struct HealthBar component.
 * Contains elements to display a healthbar.
 */
struct HealthBar : public IdentifiableComponent<HealthBar>
{
    HealthBar(
		const sf::Vector2f& size,
		const sf::Color& 	backgroundColor, 
		const sf::Color& 	fillColor, 
		const float 		fillRate = 0.f);

	sf::RectangleShape background;	//!< Background shape of the bar. 
	sf::RectangleShape bar;			//!< Bar itself, size may vary according to health.
};