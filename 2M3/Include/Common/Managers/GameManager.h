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

    void clearAll();

    // templates
    template <typename... System>
    void addLogicSystems();

    template <typename... System>
    void addRenderSystems();

    template <typename... System>
    void addNetworkSystems();

    template <ModuleID id, typename Module>
    void setModule();

    template <ModuleID id>
    Module<id>* getModule();

private:
    /**
     * Methods
     */
    void applyEntitiesToRemove();
    void removeFromSystemsLists(Entity* entity);

    /**
     * Attributes
     */
    std::unordered_map<std::uint32_t, std::shared_ptr<Entity>>  entities;

    std::set<Entity*> entitiesToRemove;

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
    std::unordered_map<ModuleID, std::unique_ptr<IModule>> modules;
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

template <ModuleID id, typename Module>
void GameManager::setModule()
{
    modules[id] = std::make_unique<Module>();
}

template <ModuleID id>
Module<id>* GameManager::getModule()
{
    return static_cast<Module<id>*>(modules[id].get());
}