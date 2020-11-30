#pragma once
#include "Common/Components/Component.h"

#include <SFML/System/Vector2.hpp>

struct Kinematics : public IdentifiableComponent<Kinematics>
{
    Kinematics() = default;

    float           angularVelocity = 0.f;
    float           angularAcceleration = 0.f;

	sf::Vector2f    velocity = sf::Vector2f(0.f, 0.f);
    sf::Vector2f    acceleration = sf::Vector2f(0.f, 0.f);
};