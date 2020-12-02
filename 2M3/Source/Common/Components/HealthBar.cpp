#include "Common/Components/HealthBar.h"
#include <algorithm>

HealthBar::HealthBar(
    const sf::Vector2f& size,
    const sf::Color& backgroundColor, 
    const sf::Color& fillColor, 
    float fillRate
    ) :
    background(size),
    bar(size)
{
    fillRate = std::clamp(fillRate, 0.f, 1.f);

    background.setFillColor(backgroundColor);
    background.setOrigin(sf::Vector2f(size.x/2.0f, size.y/2.0f));

    bar.setFillColor(fillColor);
    bar.setSize(sf::Vector2f(size.x * fillRate, size.y));
}
