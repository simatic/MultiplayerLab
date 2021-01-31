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

    /**
     * Update network and logic systems.
     * @param dt Time since last frame.
     */
    void update(const sf::Time& dt) override;

    /**
     * Add network system. Ownership is transferred to GameManager.
     * @param system System to add.
     */
    void addNetworkSystem(std::unique_ptr<System<SystemType::ClientNetwork>> system);

    /**
     * Add several network systems.
     * @tparam The systems to be added.
     */
    template <typename... System>
    void addNetworkSystems();

protected:
    /**
     * Assign an entity to its systems.
     * @param entity Entity to assign.
     */
    void updateSystemLists(Entity* entity) override;

    /**
     * Add system. Ownership is transferred to GameManager.
     * @param system System to add.
     */
    void removeFromSystemsLists(Entity* entity) override;

private:
    std::vector<std::unique_ptr<System<SystemType::ClientNetwork>>>  networkSystems; //!< The list of logic systems.

    ClientNetworkModule* networkModule; //!< Network module for network access from systems.
};

template <typename... System>
void ClientGameManager::addNetworkSystems() {
    (addNetworkSystem(std::make_unique<System>(this, networkModule)), ...);
}