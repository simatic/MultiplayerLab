#include "Common/Components/HealthBar.h"
#include <algorithm>

HealthBar::HealthBar(
    const sf::Vector2f& origin, 
    const sf::Vector2f& size, 
    const sf::Color& backgroundColor, 
    const sf::Color& fillColor, 
    float fillRate = 0.f)
    :
    background(size),
    bar(size)
{
    fillRate = std::clamp(fillRate, 0.f, 100.f);

    background.setFillColor(backgroundColor);
    background.setOrigin(origin);

    bar.setFillColor(fillColor);
    bar.setSize(sf::Vector2f(size.x * fillRate / 100.f, size.y));
}
