#include "Common/Components/Particles.h"

Particles::Particles(
    const sf::Vector2f& position,
    const sf::Color& color,
    const sf::Time& lifetime,
    const sf::Time& spawnDelay
    ) :
    position(position),
    color(color),
    lifetime(lifetime),
    spawnDelay(spawnDelay),
    shapes(sf::Quads),
    particlesDeque()
{}
