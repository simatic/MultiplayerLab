#pragma once

#include <Common/Managers/GameManager.h>
#include <Server/Modules/ServerNetworkModule.h>

class ServerGameManager : public GameManager {
public:
    ServerGameManager(ServerNetworkModule* networkModule);

    void clearAll() override;

    void update(const sf::Time& dt) override;

    void addNetworkSystem(std::unique_ptr<System<SystemType::ServerNetwork>> system);

    template <typename... System>
    void addNetworkSystems();

protected:
    void updateSystemLists(Entity* entity) override;
    void removeFromSystemsLists(Entity* entity) override;

private:
    std::vector<std::unique_ptr<System<SystemType::ServerNetwork>>>  networkSystems;

    ServerNetworkModule* networkModule;
};

template <typename... System>
void ServerGameManager::addNetworkSystems() {
    (addNetworkSystem(std::make_unique<System>(this, networkModule)), ...);
}