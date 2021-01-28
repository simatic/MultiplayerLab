#include <Common/Managers/GameManager.h>
#include <Client/Systems/NetworkSetTransform.h>
#include <Network/SetTransformPacket.h>
#include <VehicleEngine.h>
#include <Utility.h>

// Exemple de traitement d'un packet permettant de bouger une entité

NetworkSetTransform::NetworkSetTransform(GameManager* const gameManager, ClientNetworkModule* const networkModule) :
	ClientNetworkSystem<Transform>(gameManager, networkModule)
{}

void NetworkSetTransform::update(const sf::Time& dt) {
	if (!networkModule->isBufferEmpty()) {
	    auto packets = networkModule->extractPacketsOfType<SetTransformPacket>();
		while(!packets.empty()) {
			auto setTransformPacket = packets.fetchPacket();

		    auto entityID = setTransformPacket->getEntityID();
		    auto position = sf::Vector2f(setTransformPacket->getX(), setTransformPacket->getY());
		    auto angle = setTransformPacket->getAngle();

		    std::shared_ptr<Entity> entity = gameManager->getEntityWithID(entityID);
		    if(!entity)
                continue;
		    if(auto transform = entity->getComponent<Transform>()) {
		        transform->position = position;
		        transform->rotation = angle;
		    }

		    if(auto engine = entity->getComponent<VehicleEngine>()) {
		        sf::Vector2f newDirection = {std::cos(toRadians(angle)), std::sin(toRadians(angle))};
		        engine->direction = newDirection;
            }
		}
	}
}