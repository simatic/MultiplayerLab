#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include <Client/Modules/ClientNetworkModule.h>

#include <Common/Network/PingPongPackets.h>
#include <Kinematics.h>

/**
 * @class NetworkSetSpeed
 * Sets the speed of entities.
 */
// declares Kinematics as a dependency, but will actually need to convert from an entity ID to a entity,
// so this system will require to check component existence by itself
class NetworkSetSpeed : public ClientNetworkSystem<Transform, Kinematics>
{
public:
	explicit NetworkSetSpeed(GameManager* const gameManager, ClientNetworkModule* const networkModule);

	void update(const sf::Time& dt) override;
};