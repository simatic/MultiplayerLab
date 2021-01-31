#pragma once

#include <Common/Managers/GameManager.h>
#include <Client/Modules/ClientNetworkModule.h>

/**
 * @class ClientGameManager
 * GameManager specialization for the client, adds support for ClientNetworkSystem.
 */
class ClientGameManager : public GameManager {
public:
    ClientGameManager(ClientNetworkModule* networkModule);

    /**
     * Deletes all entities and systems.
     */
    void clearAll() override;

    void update(const sf::Time& dt) override;

    void updateSystemLists(Entity* entity) override;
    void removeFromSystemsLists(Entity* entity) override;

    void addNetworkSystem(std::unique_ptr<System<SystemType::ClientNetwork>> system);

    template <typename... System>
    void addNetworkSystems();

private:
    std::vector<std::unique_ptr<System<SystemType::ClientNetwork>>>  networkSystems;

    ClientNetworkModule* networkModule;
};

template <typename... System>
void ClientGameManager::addNetworkSystems() {
    (addNetworkSystem(std::make_unique<System>(this, networkModule)), ...);
}