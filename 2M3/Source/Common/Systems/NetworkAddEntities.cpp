#include <Common/Managers/GameManager.h>
#include <NetworkAddEntities.h>
#include <Network/AddEntityPacket.h>
#include <Sprite.h>

NetworkAddEntities::NetworkAddEntities(GameManager* const gameManager) :
	NetworkSystem<Transform>(gameManager)
{}

static std::vector<std::shared_ptr<Entity>> forceAlive{};

void NetworkAddEntities::update(const sf::Time& dt) {
	if (!gameManager->getNetworkModule()->getBuffer().empty()) {
	    auto packets = gameManager->getNetworkModule()->getBuffer().extractPacketsOfType<AddEntityPacket>();
		while(!packets.empty()) {
		    auto addEntityPacket = std::move(packets.front());
		    packets.pop();

		    auto entityType = addEntityPacket->getEntityType();
		    auto entityID = addEntityPacket->getEntityID();

		    const bool renderable = true;
		    std::shared_ptr<Entity> entity = Prefab::create(entityType, renderable);
            if(entityType == Prefab::Type::PlayableCar) {
                if(auto clientModule = dynamic_cast<ClientNetworkModule*>(gameManager->getNetworkModule())) {
                    clientModule->setSelfEntityID(entityID);
                } else {
                    throw std::runtime_error("NetworkAddEntities should not be put on the server.");
                }
            }

            // TODO: still necessary?
            forceAlive.push_back(entity);
            if(entity) {
                gameManager->addEntityWithID(entity, entityID);
            }
		}
	}
}