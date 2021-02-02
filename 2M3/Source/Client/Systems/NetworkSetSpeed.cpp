#include <Common/Managers/GameManager.h>
#include <Client/Systems/NetworkSetSpeed.h>
#include <Network/SetSpeedPacket.h>

// Exemple 2 de traitement d'un packet permettant de bouger une entité

NetworkSetSpeed::NetworkSetSpeed(GameManager* const gameManager, ClientNetworkModule* const networkModule) :
	ClientNetworkSystem<Transform, Kinematics>(gameManager, networkModule)
{}

void NetworkSetSpeed::update(const sf::Time& dt) {
	if (!networkModule->isBufferEmpty()) {
		// TODO: Recieve velocity
	}
}