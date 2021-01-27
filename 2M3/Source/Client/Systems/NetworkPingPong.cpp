#include <Client/Systems/NetworkPingPong.h>
#include <Common/Managers/GameManager.h>

#include <Common/Network/PingPongPackets.h>

NetworkPingPong::NetworkPingPong(GameManager* const gameManager, ClientNetworkModule* const networkModule) :
	ClientNetworkSystem<Transform>(gameManager, networkModule)
{}

void NetworkPingPong::update(const sf::Time& dt) {
    timer += dt.asSeconds();
    if(timer < 1) {
        return;
    }
    timer -= 1;
	networkModule->createAndSend<PingPacket>();
	if (!networkModule->getBuffer().empty()) {
	    auto packets = networkModule->getBuffer().extractPacketsOfType<PongPacket>();
		while(!packets.empty()) {
		    auto pingPacket = std::move(packets.front());
		    packets.pop();

            // std::cout << "PONG" << std::endl;
		}
	}
}