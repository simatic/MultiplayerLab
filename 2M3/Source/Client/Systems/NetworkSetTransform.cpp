#include <Common/Managers/GameManager.h>
#include <Client/Systems/NetworkSetTransform.h>
#include <Network/SetTransformPacket.h>

// Exemple de traitement d'un packet permettant de bouger une entité

NetworkSetTransform::NetworkSetTransform(GameManager* const gameManager, INetworkModule* const networkModule) :
	NetworkSystem<Transform>(gameManager, networkModule)
{}

void NetworkSetTransform::update(const sf::Time& dt) {
	if (!gameManager->getNetworkModule()->getBuffer().empty()) {
	    auto packets = gameManager->getNetworkModule()->getBuffer().extractPacketsOfType<SetTransformPacket>();
		while(!packets.empty()) {
		    auto setTransformPacket = std::move(packets.front());
		    packets.pop();

		    auto entityID = setTransformPacket->getEntityID();
		    auto position = sf::Vector2f(setTransformPacket->getX(), setTransformPacket->getY());
		    auto angle = setTransformPacket->getAngle();

		    std::shared_ptr<Entity> entity = gameManager->getEntityWithID(entityID);
		    if(auto transform = entity->getComponent<Transform>()) {
		        transform->position = position;
		        transform->rotation = angle;
		    }
		}
	}
}