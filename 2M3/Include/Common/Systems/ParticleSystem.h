#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/CarEngine.h"
#include "Common/Components/Particles.h"

class ParticleSystem : public SignedSystem<Transform, CarEngine, Particles>
{
public:
	void update(const sf::Time& dt) override;;
};