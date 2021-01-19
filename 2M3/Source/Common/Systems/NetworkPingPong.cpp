#include <Common/Systems/NetworkPingPong.h>
#include <Common/Managers/GameManager.h>

#include <Common/Network/PingPongPackets.h>

NetworkPingPong::NetworkPingPong(GameManager* const gameManager) :
	NetworkSystem<Transform>(gameManager)
{}

void NetworkPingPong::update(const sf::Time& dt) {
	gameManager->getNetworkModule()->createAndSend<PingPacket>();
	if (!gameManager->getNetworkModule()->getBuffer().empty()) {
	    auto packets = gameManager->getNetworkModule()->getBuffer().extractPacketsOfType<PongPacket>();
		while(!packets.empty()) {
		    auto pingPacket = std::move(packets.front());
		    packets.pop();

            // std::cout << "PONG" << std::endl;
		}
	}
}