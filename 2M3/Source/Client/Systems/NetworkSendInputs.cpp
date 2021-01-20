#include <Client/Systems/NetworkSendInputs.h>
#include <Common/Managers/GameManager.h>

#include <Common/Network/PingPongPackets.h>
#include <Network/InputPacket.h>

NetworkSendInputs::NetworkSendInputs(GameManager* const gameManager) :
	NetworkSystem<Transform, PlayableCarInput>(gameManager)
{}

void NetworkSendInputs::update(const sf::Time& dt) {
    if(entities.empty())
        return;
    auto playerEntity = *entities.begin();
    CarInput* inputs = playerEntity->getComponent<PlayableCarInput>()->inputs;
	gameManager->getNetworkModule()->createAndSend<InputPacket>(inputs->up, inputs->down, inputs->left, inputs->right, inputs->action);
}