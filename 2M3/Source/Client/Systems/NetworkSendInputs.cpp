#include <Client/Systems/NetworkSendInputs.h>
#include <Common/Managers/GameManager.h>

#include <Common/Network/PingPongPackets.h>
#include <Network/InputPacket.h>

NetworkSendInputs::NetworkSendInputs(GameManager* const gameManager, INetworkModule* const networkModule) :
    NetworkSystem<Transform, PlayableCarInput>(gameManager, networkModule)
{}

void NetworkSendInputs::update(const sf::Time& dt) {
    if(entities.empty())
        return;
    auto playerEntity = *entities.begin();
    CarInput* inputs = playerEntity->getComponent<PlayableCarInput>()->inputs;
    networkModule->createAndSend<InputPacket>(inputs->up, inputs->down, inputs->left, inputs->right, inputs->action);
}