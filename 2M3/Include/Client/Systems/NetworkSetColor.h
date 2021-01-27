#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include <Client/Modules/ClientNetworkModule.h>

#include <Common/Network/PingPongPackets.h>

class NetworkSetColor : public ClientNetworkSystem<Transform>
{
public:
	explicit NetworkSetColor(GameManager* const gameManager, ClientNetworkModule* const networkModule);

	void update(const sf::Time& dt) override;
};