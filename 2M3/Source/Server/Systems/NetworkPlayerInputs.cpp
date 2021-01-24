#include <Common/Managers/GameManager.h>
#include <Server/Systems/NetworkPlayerInputs.h>
#include <Server/Modules/ServerNetworkModule.h>
#include <Server/Server.h>
#include <CarInput.h>
#include <cassert>
#include <Network/InputPacket.h>

NetworkPlayerInputs::NetworkPlayerInputs(GameManager* const gameManager, INetworkModule* const networkModule) :
        NetworkSystem<Transform>(gameManager, networkModule)
{}

void NetworkPlayerInputs::update(const sf::Time& dt) {
    if (!networkModule->isBufferEmpty()) {
        if(auto serverModule = dynamic_cast<ServerNetworkModule*>(networkModule)) {
            
            // TODO: Receive client’s inputs from server side

        } else {
            throw std::runtime_error("Cannot use NetworkPlayerInputs on a client.");
        }
    }
}