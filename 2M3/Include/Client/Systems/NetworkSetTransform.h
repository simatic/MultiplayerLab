#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include <Client/Modules/ClientNetworkModule.h>

#include <Common/Network/PingPongPackets.h>

/**
 * @class NetworkSetTransform
 * Sets transform for entities.
 */
class NetworkSetTransform : public ClientNetworkSystem<Transform>
{
public:
	explicit NetworkSetTransform(GameManager* const gameManager, ClientNetworkModule* const networkModule);

	void update(const sf::Time& dt) override;
};