#include <Common/Managers/GameManager.h>
#include <Client/Systems/NetworkAddEntities.h>
#include <Network/AddEntityPacket.h>
#include <Sprite.h>

NetworkAddEntities::NetworkAddEntities(GameManager* const gameManager, ClientNetworkModule* const networkModule) :
	ClientNetworkSystem<Transform>(gameManager, networkModule)
{}

void NetworkAddEntities::update(const sf::Time& dt) {
	if (!networkModule->isBufferEmpty()) {
	    auto packets = networkModule->extractPacketsOfType<AddEntityPacket>();
		while(!packets.empty()) {
			auto addEntityPacket = packets.fetchPacket();

		    auto entityType = addEntityPacket->getEntityType();
		    auto entityID = addEntityPacket->getEntityID();

		    const bool renderable = true;
		    std::shared_ptr<Entity> entity = Prefab::create(entityType, renderable);
            if(entityType == Prefab::Type::PlayableCar) {
				networkModule->setSelfEntityID(entityID);
            }

            if(entity) {
                gameManager->addEntityWithID(entity, entityID);
            }
		}
	}
}