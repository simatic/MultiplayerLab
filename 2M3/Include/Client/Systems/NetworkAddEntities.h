#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include <Client/Modules/ClientNetworkModule.h>

#include <Common/Network/PingPongPackets.h>

class NetworkAddEntities : public ClientNetworkSystem<Transform>
{
public:
	explicit NetworkAddEntities(GameManager* const gameManager, ClientNetworkModule* const networkModule);

	void update(const sf::Time& dt) override;
};