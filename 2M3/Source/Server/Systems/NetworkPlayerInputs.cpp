#include <Common/Managers/GameManager.h>
#include <Server/Systems/NetworkPlayerInputs.h>
#include <Server/Modules/ServerNetworkModule.h>
#include <Server/Server.h>
#include <CarInput.h>
#include <cassert>
#include <Network/InputPacket.h>

NetworkPlayerInputs::NetworkPlayerInputs(GameManager* const gameManager, ServerNetworkModule* const networkModule) :
        ServerNetworkSystem<Transform>(gameManager, networkModule)
{}

void NetworkPlayerInputs::update(const sf::Time& dt) {
    if (!networkModule->isBufferEmpty()) {
        auto packets = networkModule->extractPacketsOfType<InputPacket>();
        while(!packets.empty()) {
            auto inputPacket = std::move(packets.front());
            packets.pop();

            // TODO: re-broadcast to clients to help their predictions

            ClientID client = inputPacket->getSender();

            auto playerEntity = networkModule->getServer().getClientEntityByID(client);
            if(playerEntity) {
                auto inputs = playerEntity->getComponent<CarInput>();
                assert(inputs != nullptr);
                inputs->action = inputPacket->getFire();
                inputs->left = inputPacket->getLeft();
                inputs->right = inputPacket->getRight();
                inputs->up = inputPacket->getUp();
                inputs->down = inputPacket->getDown();
            }
        }
    }
}