#include <Server/Systems/NetworkUpdatePositionsAndVelocities.h>
#include <Common/Managers/GameManager.h>
#include <Server/Modules/ServerNetworkModule.h>
#include <Server/Server.h>
#include <Network/InputPacket.h>

NetworkUpdatePositionsAndVelocities::NetworkUpdatePositionsAndVelocities(GameManager* const gameManager) :
        NetworkSystem<Transform, Kinematics>(gameManager)
{}

void NetworkUpdatePositionsAndVelocities::update(const sf::Time& dt) {
    if(auto serverModule = dynamic_cast<ServerNetworkModule*>(gameManager->getNetworkModule())) {
        auto& clients = serverModule->getServer().getClients();
        // TODO: envoi des packets correspondants
        // for each entity in entities
    } else {
        throw std::runtime_error("Cannot use NetworkUpdatePositionsAndVelocities on a client.");
    }
}