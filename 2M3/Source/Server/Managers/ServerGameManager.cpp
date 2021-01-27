#include <Server/Managers/ServerGameManager.h>

ServerGameManager::ServerGameManager(ServerNetworkModule* networkModule) :
    networkModule(networkModule)
{}

void ServerGameManager::clearAll() {
    networkSystems.clear();
    GameManager::clearAll();
}

void ServerGameManager::update(const sf::Time& dt) {
    for (const auto& system : networkSystems) {
        system->update(dt);
    }
    GameManager::update(dt);
}

void ServerGameManager::addNetworkSystem(std::unique_ptr<System<SystemType::ServerNetwork>> system) {
    networkSystems.push_back(std::move(system));
}

void ServerGameManager::updateSystemLists(Entity* entity) {
    for (auto& system : networkSystems) {
        if ((entity->getSignature() & system->getSignature()) == system->getSignature()) {
            system->addEntity(entity);
        }
        else {
            system->removeEntity(entity);
        }
    }
    GameManager::updateSystemLists(entity);
}

void ServerGameManager::removeFromSystemsLists(Entity* entity) {
    for (auto& system : networkSystems) {
        system->removeEntity(entity);
    }
    GameManager::removeFromSystemsLists(entity);
}

