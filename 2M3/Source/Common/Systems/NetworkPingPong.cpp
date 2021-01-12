#include <Common/Systems/NetworkPingPong.h>
#include <Common/Managers/GameManager.h>

#include <Common/Network/PingPongPackets.h>

NetworkPingPong::NetworkPingPong(GameManager* const gameManager) :
	NetworkSystem<Transform>(gameManager)
{}

void NetworkPingPong::update(const sf::Time& dt) {
	gameManager->getNetworkModule()->createAndSend<PingPacket>();
	if (!gameManager->getNetworkModule()->getBuffer().empty()) {
		if (std::unique_ptr<Packet> packet = gameManager->getNetworkModule()->getBuffer().fetchPacket()) {
			if (packet->getID() == PacketID::Pong) {
				std::cout << "PONG" << std::endl;
			}
		}
	}
}