#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include <Client/Modules/ClientNetworkModule.h>

class NetworkPlayerInputs : public ServerNetworkSystem<Transform>
{
public:
    explicit NetworkPlayerInputs(GameManager* const gameManager, ServerNetworkModule* const networkModule);

    void update(const sf::Time& dt) override;
};