#include <Client/Systems/NetworkSendInputs.h>
#include <Common/Managers/GameManager.h>

#include <Common/Network/PingPongPackets.h>
#include <Network/InputPacket.h>

NetworkSendInputs::NetworkSendInputs(GameManager* const gameManager, ClientNetworkModule* const networkModule) :
    ClientNetworkSystem<Transform, PlayableVehicleInput>(gameManager, networkModule)
{}

void NetworkSendInputs::update(const sf::Time& dt) {
    if(entities.empty())
        return;
    auto playerEntity = *entities.begin();
    PlayableVehicleInput* inputs = playerEntity->getComponent<PlayableVehicleInput>();
    networkModule->createAndSend<InputPacket>(inputs);
}