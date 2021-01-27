#include <Client/Managers/ClientGameManager.h>

ClientGameManager::ClientGameManager(ClientNetworkModule* networkModule) :
    networkModule(networkModule)
{}

void ClientGameManager::clearAll() {
    networkSystems.clear();
    GameManager::clearAll();
}

void ClientGameManager::update(const sf::Time& dt) {
    for (std::unique_ptr<System<SystemType::ClientNetwork>>& system : networkSystems) {
        system->update(dt);
    }
    GameManager::update(dt);
}

void ClientGameManager::addNetworkSystem(std::unique_ptr<System<SystemType::ClientNetwork>> system) {
    networkSystems.push_back(std::move(system));
}

void ClientGameManager::updateSystemLists(Entity* entity) {
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

void ClientGameManager::removeFromSystemsLists(Entity* entity) {
    for (auto& system : networkSystems) {
        system->removeEntity(entity);
    }
    GameManager::removeFromSystemsLists(entity);
}

