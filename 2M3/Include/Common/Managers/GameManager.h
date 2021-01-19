#pragma once
#include "Common/Systems/System.h"
#include "Common/Entity.h"
#include "KeyBinding.h"
#include <Common/Modules/INetworkModule.h>

#include <Common/Network/Constants.h>

#include <memory>
#include <vector>
#include <queue>

#include <SFML/Graphics/RenderTarget.hpp>

class GameManager
{
public:
    GameManager() = default;

    /**
     * Methods
     */
    void addEntity(std::shared_ptr<Entity> entity);
    void addEntityWithID(std::shared_ptr<Entity> entity, std::uint32_t id);
    void addEntityWithIDNextFrame(std::shared_ptr<Entity> entity, std::uint32_t id);
    std::shared_ptr<Entity> getEntityWithID(std::uint32_t id);
    void removeEntityNextFrame(Entity* entity);
    void removeEntity(Entity* entity);

    void update(const sf::Time& dt);
    void render(const sf::Time& dt);

    void addLogicSystem(std::unique_ptr<System<SystemType::Logic>> system);
    void addRenderSystem(std::unique_ptr<System<SystemType::Render>> system);
    void addNetworkSystem(std::unique_ptr<System<SystemType::Network>> system);

    void updateSystemLists();
    void updateSystemLists(Entity* entity);

    sf::RenderTarget* getRenderTarget() const;
    void              setRenderTarget(sf::RenderTarget* target);

    KeyBinding*       getKeyBinding() const;
    void              setKeyBinding(KeyBinding* keys);

    template <typename Module, typename... Args>
    void            setNetworkModule(Args... args);
    INetworkModule* getNetworkModule() const;

    void clearAll();

    // templates
    template <typename... System>
    void addLogicSystems();

    template <typename... System>
    void addRenderSystems();

    template <typename... System>
    void addNetworkSystems();

    std::vector<std::shared_ptr<Entity>> getEntityList() const;

private:
    /**
     * Methods
     */
    void applyEntitiesToRemove();
    void applyEntitiesToAdd();
    void removeFromSystemsLists(Entity* entity);

    /**
     * Attributes
     */
    std::unordered_map<std::uint32_t, std::shared_ptr<Entity>>  entities{};

    std::set<Entity*> entitiesToRemove{};
    std::map<std::uint32_t, std::shared_ptr<Entity>> entitiesToAdd{};

    std::vector<std::unique_ptr<System<SystemType::Logic>>>    logicSystems;
    std::vector<std::unique_ptr<System<SystemType::Render>>>   renderSystems;
    std::vector<std::unique_ptr<System<SystemType::Network>>>  networkSystems;

    std::uint32_t               highestID = 1;
    std::queue<std::uint32_t>   unusedIDs;

    sf::RenderTarget*	target = nullptr;
    KeyBinding*         keyBinding = nullptr;

    /**
    * Modules
    */
    std::unique_ptr<INetworkModule> networkModule = nullptr;
};

template <typename... System>
void GameManager::addLogicSystems()
{
    (addLogicSystem(std::make_unique<System>(this)), ...);
}

template <typename... System>
void GameManager::addRenderSystems()
{
    (addRenderSystem(std::make_unique<System>(this)), ...);
}

template <typename... System>
void GameManager::addNetworkSystems()
{
    (addNetworkSystem(std::make_unique<System>(this)), ...);
}

template <typename Module, typename... Args>
void GameManager::setNetworkModule(Args... args)
{
    networkModule = std::make_unique<Module>(args...);
}