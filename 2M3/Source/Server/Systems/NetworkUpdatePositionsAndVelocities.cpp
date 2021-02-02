#include <Server/Systems/NetworkUpdatePositionsAndVelocities.h>
#include <Common/Managers/GameManager.h>
#include <Server/Modules/ServerNetworkModule.h>
#include <Server/Server.h>
#include <Network/InputPacket.h>
#include <Common/Network/SetTransformPacket.h>
#include <Common/Network/SetSpeedPacket.h>

NetworkUpdatePositionsAndVelocities::NetworkUpdatePositionsAndVelocities(GameManager* const gameManager, ServerNetworkModule* const networkModule) :
        ServerNetworkSystem<Transform, Kinematics>(gameManager, networkModule)
{}

void NetworkUpdatePositionsAndVelocities::update(const sf::Time& dt) {
    timer += dt.asSeconds();
    if(timer < 0.02) {
        return;
    }
    timer -= 0.02;
   
    // TODO: Send Transform and velocity to clients
}