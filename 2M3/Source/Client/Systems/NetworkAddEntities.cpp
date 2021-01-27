#include <Common/Managers/GameManager.h>
#include <Client/Systems/NetworkAddEntities.h>
#include <Network/AddEntityPacket.h>
#include <Sprite.h>

NetworkAddEntities::NetworkAddEntities(GameManager* const gameManager, ClientNetworkModule* const networkModule) :
	ClientNetworkSystem<Transform>(gameManager, networkModule)
{}

static std::vector<std::shared_ptr<Entity>> forceAlive{};

void NetworkAddEntities::update(const sf::Time& dt) {
	if (!networkModule->isBufferEmpty()) {
	    auto packets = networkModule->extractPacketsOfType<AddEntityPacket>();
		while(!packets.empty()) {
		    auto addEntityPacket = std::move(packets.front());
		    packets.pop();

		    auto entityType = addEntityPacket->getEntityType();
		    auto entityID = addEntityPacket->getEntityID();

		    const bool renderable = true;
		    std::shared_ptr<Entity> entity = Prefab::create(entityType, renderable);
            if(entityType == Prefab::Type::PlayableCar) {
				networkModule->setSelfEntityID(entityID);
            }

            // TODO: still necessary?
     //       forceAlive.push_back(entity);
            if(entity) {
                gameManager->addEntityWithID(entity, entityID);
            }
		}
	}
}