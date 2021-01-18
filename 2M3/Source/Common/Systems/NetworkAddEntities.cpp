#include <Common/Managers/GameManager.h>
#include <NetworkAddEntities.h>
#include <Network/AddEntityPacket.h>

NetworkAddEntities::NetworkAddEntities(GameManager* const gameManager) :
	NetworkSystem<Transform>(gameManager)
{}

void NetworkAddEntities::update(const sf::Time& dt) {
	if (!gameManager->getNetworkModule()->getBuffer().empty()) {
	    auto packets = gameManager->getNetworkModule()->getBuffer().extractPacketsOfType<AddEntityPacket>();
		while(!packets.empty()) {
		    auto addEntityPacket = std::move(packets.front());
		    packets.pop();

		    auto entityType = addEntityPacket->getEntityType();
		    auto entityID = addEntityPacket->getEntityID();

		    const bool renderable = true;
		    std::shared_ptr<Entity> entity = nullptr;
            switch (entityType) {
                case Prefab::playableCar: {
                    entity = Prefab::createPlayableCar(renderable);
                } break;

                case Prefab::car: {
                    entity = Prefab::createCar(renderable);
                } break;

                case Prefab::bullet: {
                    entity = Prefab::createBullet(renderable);
                } break;
            }

            if(entity) {
                gameManager->addEntityWithID(entity, entityID);
                // TODO: position & such
            }
		}
	}
}