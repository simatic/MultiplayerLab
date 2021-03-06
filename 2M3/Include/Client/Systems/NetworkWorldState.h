#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include <Client/Modules/ClientNetworkModule.h>

#include <Common/Network/WorldStatePacket.h>

/**
 * @class NetworkWorldState
 * Updates the world state from the server.
 */
class NetworkWorldState : public ClientNetworkSystem<Transform>
{
public:
	explicit NetworkWorldState(GameManager* const gameManager, ClientNetworkModule* const networkModule);

	void update(const sf::Time& dt) override;

private:
    void applyWorldStateEntities(const std::vector<WorldStatePacket::EntityInformation>& entities);
};