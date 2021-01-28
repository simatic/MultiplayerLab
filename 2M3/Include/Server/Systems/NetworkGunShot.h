#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/CarInput.h"
#include "Common/Components/CarEngine.h"
#include "Common/Components/Gun.h"

class NetworkGunShot : public ServerNetworkSystem<CarInput, CarEngine, Gun>
{
public:
	explicit NetworkGunShot(GameManager* const gameManager, ServerNetworkModule* networkModule);

	void update(const sf::Time& dt) override;;
};