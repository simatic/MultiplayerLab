#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/VehicleEngine.h"
#include "Common/Components/Particles.h"

/**
 * @class ParticleRenderer
 * Generate particle at the back of the vehicle.
 */
class ParticleSystem : public LogicSystem<Transform, VehicleEngine, Particles>
{
public:
	explicit ParticleSystem(GameManager* const gameManager);

	void update(const sf::Time& dt) override;;
};