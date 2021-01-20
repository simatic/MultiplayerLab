#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include <Client/Modules/ClientNetworkModule.h>

#include <Common/Network/PingPongPackets.h>

class NetworkSetColor : public NetworkSystem<Transform>
{
public:
	explicit NetworkSetColor(GameManager* const gameManager);

	void update(const sf::Time& dt) override;
};