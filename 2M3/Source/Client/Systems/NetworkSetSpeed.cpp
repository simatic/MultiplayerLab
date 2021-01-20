#include <Common/Managers/GameManager.h>
#include <Client/Systems/NetworkSetSpeed.h>
#include <Network/SetSpeedPacket.h>

// Exemple 2 de traitement d'un packet permettant de bouger une entité

NetworkSetSpeed::NetworkSetSpeed(GameManager* const gameManager) :
	NetworkSystem<Transform, Kinematics>(gameManager)
{}

void NetworkSetSpeed::update(const sf::Time& dt) {
	if (!gameManager->getNetworkModule()->getBuffer().empty()) {
	    auto packets = gameManager->getNetworkModule()->getBuffer().extractPacketsOfType<SetSpeedPacket>();
		while(!packets.empty()) {
		    auto setSpeedPacket = std::move(packets.front());
		    packets.pop();

		    auto entityID = setSpeedPacket->getEntityID();
		    auto speed = sf::Vector2f(setSpeedPacket->getSpeedX(), setSpeedPacket->getSpeedY());

		    std::shared_ptr<Entity> entity = gameManager->getEntityWithID(entityID);
		    if(auto kinematics = entity->getComponent<Kinematics>()) {
                kinematics->velocity = speed;
		    }
		}
	}
}