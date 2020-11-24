#pragma once
#include <deque>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include "Common/Components/Component.h"

struct Particle
{
	sf::Vector2f	position;
	sf::Color		color;
	sf::Time		lifetime;
};

struct Particles : public IdentifiableComponent<Particles>
{
    Particles(
        const sf::Vector2f& position,
        const sf::Color& color,
        const sf::Time& lifetime,
        const sf::Time& spawnDelay);

	sf::Vector2f position;
    sf::Color color;
    sf::Time lifetime;

    sf::Time spawnDelay;

    sf::VertexArray shapes;
    std::deque<Particle> particlesDeque;

    sf::Time currentDelay = sf::seconds(0);
};