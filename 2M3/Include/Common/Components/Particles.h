#pragma once
#include "Common/Components/Component.h"

#include <deque>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

/**
 * @struct Particle.
 */
struct Particle
{
	sf::Vector2f	position;
	sf::Color		color;
	sf::Time		lifetime;
};

/**
 * @struct Particles component.
 * Component to render particles.
 */
struct Particles : public IdentifiableComponent<Particles>
{
    Particles(
        const sf::Vector2f& position,
        const sf::Color&    color,
        const sf::Time&     lifetime,
        const sf::Time&     spawnDelay);

	sf::Vector2f            position;       //!< Particles position.
    sf::Color               color;          //!< Particles color.
    sf::Time                lifetime;       //!< Particles lifetime.

    sf::Time                spawnDelay;     //!< Particles delay between each spawn.

    sf::VertexArray         shapes;         //!< Particles shape.
    std::deque<Particle>    particlesDeque; //!< Particles set.

    sf::Time                currentDelay = sf::seconds(0);
};