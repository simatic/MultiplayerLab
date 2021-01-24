#include <Client/Systems/NetworkWorldState.h>
#include <Common/Managers/GameManager.h>

NetworkWorldState::NetworkWorldState(GameManager* const gameManager, INetworkModule* const networkModule) :
	NetworkSystem<Transform>(gameManager, networkModule)
{}

void NetworkWorldState::applyWorldStateEntities(const std::vector<WorldStatePacket::EntityInformation>& entities) {
    for(const auto& info : entities) {
        auto entityID = info.id;
        auto entity = gameManager->getEntityWithID(entityID);
        if(!entity) { // the entity does not exist yet, create it
            entity = Prefab::create(info.type, true);
            gameManager->addEntityWithID(entity, entityID);
        }

        if(info.hasTransform) {
            if(auto transform = entity->getComponent<Transform>()) {
                transform->position = sf::Vector2f(info.x, info.y);
                transform->rotation = info.angle;
            } else {
                throw std::runtime_error("Because `info.hasTransform` is `true`, expected entity with Transform component, but none could be found!");
            }
        }
    }
}

void NetworkWorldState::update(const sf::Time& dt) {
	if (!networkModule->isBufferEmpty()) {
	    auto packets = networkModule->extractPacketsOfType<WorldStatePacket>();
		while(!packets.empty()) {
		    auto worldStatePacket = std::move(packets.front());
		    packets.pop();

            applyWorldStateEntities(worldStatePacket->getEntityInformation());
		}
	}
}