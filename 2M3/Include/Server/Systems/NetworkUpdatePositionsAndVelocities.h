#pragma once

#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include <Client/Modules/ClientNetworkModule.h>
#include <Kinematics.h>

class NetworkUpdatePositionsAndVelocities : public ServerNetworkSystem<Transform, Kinematics>
{
private:
    float timer = 0;

public:
    explicit NetworkUpdatePositionsAndVelocities(GameManager* const gameManager, ServerNetworkModule* const networkModule);

    void update(const sf::Time& dt) override;
};