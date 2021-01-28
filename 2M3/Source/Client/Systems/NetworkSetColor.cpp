#include <Common/Managers/GameManager.h>
#include <Client/Systems/NetworkSetColor.h>
#include <Network/SetColorPacket.h>
#include <Sprite.h>

NetworkSetColor::NetworkSetColor(GameManager* const gameManager, ClientNetworkModule* const networkModule) :
	ClientNetworkSystem<Transform>(gameManager, networkModule)
{}

void NetworkSetColor::update(const sf::Time& dt) {
	if (!networkModule->isBufferEmpty()) {
	    auto packets = networkModule->extractPacketsOfType<SetColorPacket>();
		while(!packets.empty()) {
			auto setColorPacket = packets.fetchPacket();

		    auto entityID = setColorPacket->getEntityID();

		    auto playerEntityID = networkModule->getSelfEntityID();

		    // FIXME: possibly packet-order-dependent: requires player car to have been already added to the world (before this frame or before inside this frame)
		    if(entityID == playerEntityID) {
                networkModule->getClientContext().associatedColor = setColorPacket->getColor();
            }

		    auto entity = gameManager->getEntityWithID(entityID);
		    if(entity) {
		        if(auto sprite = entity->getComponent<Sprite>()) {
		            sprite->colorFilter = setColorPacket->getColor();
		            std::cerr << "Change color to " << std::to_string(sprite->colorFilter.r) << std::endl;
		        }
			}
			else {
				throw std::runtime_error("Could not find entity with id " + std::to_string(entityID) + " inside " + std::string(__FILE__) + ":" + std::to_string(__LINE__));
			}
		}
	}
}