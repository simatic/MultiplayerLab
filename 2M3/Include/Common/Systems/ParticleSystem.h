#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/CarEngine.h"
#include "Common/Components/Particles.h"

class ParticleSystem : public LogicSystem<Transform, CarEngine, Particles>
{
public:
	explicit ParticleSystem(GameManager* const gameManager);

	void update(const sf::Time& dt) override;;
};