#include <Common/Managers/GameManager.h>
#include <Client/Systems/NetworkSetSpeed.h>
#include <Network/SetSpeedPacket.h>

// Exemple 2 de traitement d'un packet permettant de bouger une entité

NetworkSetSpeed::NetworkSetSpeed(GameManager* const gameManager, ClientNetworkModule* const networkModule) :
	ClientNetworkSystem<Transform, Kinematics>(gameManager, networkModule)
{}

void NetworkSetSpeed::update(const sf::Time& dt) {
	if (!networkModule->isBufferEmpty()) {
	    auto packets = networkModule->extractPacketsOfType<SetVelocityPacket>();
		while(!packets.empty()) {
			auto setSpeedPacket = packets.fetchPacket();

		    auto entityID = setSpeedPacket->getEntityID();
		    auto speed = sf::Vector2f(setSpeedPacket->getSpeedX(), setSpeedPacket->getSpeedY());

		    std::shared_ptr<Entity> entity = gameManager->getEntityWithID(entityID);
		    if(!entity)
                continue;
		    if(auto kinematics = entity->getComponent<Kinematics>()) {
                kinematics->velocity = speed;
		    }
		}
	}
}