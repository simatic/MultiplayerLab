#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include <Client/Modules/ClientNetworkModule.h>

#include <Common/Network/PingPongPackets.h>

class NetworkAddEntities : public NetworkSystem<Transform>
{
public:
	explicit NetworkAddEntities(GameManager* const gameManager, INetworkModule* const networkModule);

	void update(const sf::Time& dt) override;
};