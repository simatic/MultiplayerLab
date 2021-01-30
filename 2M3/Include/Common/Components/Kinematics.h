#pragma once
#include "Common/Components/Component.h"

#include <SFML/System/Vector2.hpp>

/**
 * @struct Kinematics component.
 * Holds kinematics properties such as velocity and acceleration.
 */
struct Kinematics : public IdentifiableComponent<Kinematics>
{
    Kinematics() = default;

    float           angularVelocity = 0.f;                  //!< Angular velocity (commonly in rad/s or deg/s)
    float           angularAcceleration = 0.f;              //!< Angular acceleration (commonly in rad/s² or deg/s²)

	sf::Vector2f    velocity = sf::Vector2f(0.f, 0.f);      //!< Velocity (in units/s)
    sf::Vector2f    acceleration = sf::Vector2f(0.f, 0.f);  //!< Acceleration (in units/s²)
};