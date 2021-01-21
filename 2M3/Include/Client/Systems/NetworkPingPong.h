#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include <Client/Modules/ClientNetworkModule.h>

#include <Common/Network/PingPongPackets.h>

class NetworkPingPong : public NetworkSystem<Transform>
{
private:
    float timer;

public:
	explicit NetworkPingPong(GameManager* const gameManager);

	void update(const sf::Time& dt) override;
};