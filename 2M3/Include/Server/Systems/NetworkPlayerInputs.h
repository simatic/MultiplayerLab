#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include <Client/Modules/ClientNetworkModule.h>

class NetworkPlayerInputs : public NetworkSystem<Transform>
{
public:
    explicit NetworkPlayerInputs(GameManager* const gameManager, INetworkModule* const networkModule);

    void update(const sf::Time& dt) override;
};