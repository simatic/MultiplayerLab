#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include <Client/Modules/ClientNetworkModule.h>

#include <Common/Network/PingPongPackets.h>
#include <PlayableCarInput.h>

class NetworkSendInputs : public ClientNetworkSystem<Transform, PlayableCarInput>
{
public:
	explicit NetworkSendInputs(GameManager* const gameManager, ClientNetworkModule* const networkModule);

	void update(const sf::Time& dt) override;
};