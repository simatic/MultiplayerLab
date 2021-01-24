#include <Client/Systems/NetworkPingPong.h>
#include <Common/Managers/GameManager.h>

#include <Common/Network/PingPongPackets.h>

NetworkPingPong::NetworkPingPong(GameManager* const gameManager, INetworkModule* const networkModule) :
	NetworkSystem<Transform>(gameManager, networkModule)
{}

void NetworkPingPong::update(const sf::Time& dt) {
    timer += dt.asSeconds();
    if(timer < 1) {
        return;
    }
    timer -= 1;
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