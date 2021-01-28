#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/CarInput.h"
#include "Common/Components/VehicleEngine.h"
#include "Common/Components/Gun.h"

class NetworkGunShot : public ServerNetworkSystem<CarInput, VehicleEngine, Gun>
{
public:
	explicit NetworkGunShot(GameManager* const gameManager, ServerNetworkModule* networkModule);

	void update(const sf::Time& dt) override;;
};