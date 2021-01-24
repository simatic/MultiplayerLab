#include <Common/Managers/GameManager.h>
#include <Client/Systems/NetworkSetColor.h>
#include <Network/SetColorPacket.h>
#include <Sprite.h>

NetworkSetColor::NetworkSetColor(GameManager* const gameManager, INetworkModule* const networkModule) :
	NetworkSystem<Transform>(gameManager, networkModule)
{}

void NetworkSetColor::update(const sf::Time& dt) {
	if (!gameManager->getNetworkModule()->getBuffer().empty()) {
	    auto packets = gameManager->getNetworkModule()->getBuffer().extractPacketsOfType<SetColorPacket>();
		while(!packets.empty()) {
		    auto setColorPacket = std::move(packets.front());
		    packets.pop();

		    auto entityID = setColorPacket->getEntityID();

		    if(auto clientModule = dynamic_cast<ClientNetworkModule*>(gameManager->getNetworkModule())) {
		        auto playerEntityID = clientModule->getSelfEntityID();

		        // FIXME: possibly packet-order-dependent: requires player car to have been already added to the world (before this frame or before inside this frame)
		        if(entityID == playerEntityID) {
                    clientModule->getClientContext().associatedColor = setColorPacket->getColor();
                }

		        auto entity = gameManager->getEntityWithID(entityID);
		        if(entity) {
		            if(auto sprite = entity->getComponent<Sprite>()) {
		                sprite->colorFilter = setColorPacket->getColor();
		                std::cerr << "Change color to " << std::to_string(sprite->colorFilter.r) << std::endl;
		            }
		        } else {
                    throw std::runtime_error("Could not find entity with id "+std::to_string(entityID)+" inside "+std::string(__FILE__)+":"+std::to_string(__LINE__));
		        }
		    } else {
		        throw std::runtime_error("NetworkSetColor should not be put on the server");
		    }
		}
	}
}