#pragma once
#include <SFML/System/Time.hpp>
#include "Common/Systems/System.h"
#include "Car.h"
#include "Common/Components/Particles.h"

class ParticleSystem : public SignedSystem<Transform, Particles>
{
public:
	static void update(Car* entity, const sf::Time& dt);
};