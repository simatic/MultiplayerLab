#pragma once
#include <Common/Entity.h>
#include <Common/Systems/System.h>
#include <KeyBinding.h>
#include <Common/Modules/INetworkModule.h>
#include <Common/Network/Constants.h>

#include <memory>
#include <vector>
#include <queue>
#include <functional>

#include <SFML/Graphics/RenderTarget.hpp>

/**
 * @class GameManager
 * The GameManager contains the list of entities and systems that are used while the game is launched.
 * It serves as an interface between the SFML Application and the game engine.
 */
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
    std::shared_ptr<Entity> getEntityWithID(std::uint32_t id) const;
    void removeEntityNextFrame(Entity* entity);
    void removeEntity(Entity* entity);

    virtual void update(const sf::Time& dt);
    void render(const sf::Time& dt);

    void addLogicSystem(std::unique_ptr<System<SystemType::Logic>> system);
    void addRenderSystem(std::unique_ptr<System<SystemType::Render>> system);

    virtual void updateSystemListsForAllEntities();
    virtual void updateSystemLists(Entity* entity);

    sf::RenderTarget* getRenderTarget() const;
    void              setRenderTarget(sf::RenderTarget* target);

    KeyBinding*       getKeyBinding() const;
    void              setKeyBinding(KeyBinding* keys);

    virtual void clearAll();

    // templates
    template <typename... System>
    void addLogicSystems();

    template <typename... System>
    void addRenderSystems();

    void nextFrame(std::function<void()> actionToDo);

    std::vector<std::shared_ptr<Entity>> getEntityList() const;

protected:
    /**
     * Methods
     */
    void applyEntitiesToRemove();
    void applyEntitiesToAdd();
    virtual void removeFromSystemsLists(Entity* entity);

    /**
     * Attributes
     */
    std::unordered_map<std::uint32_t, std::shared_ptr<Entity>>  entities{}; //!< The list of entities in the game.

    std::set<Entity*> entitiesToRemove{};                               //!< Entities to be removed next frame.
    std::map<std::uint32_t, std::shared_ptr<Entity>> entitiesToAdd{};   //!< Entities to be added next frame.

    std::vector<std::unique_ptr<System<SystemType::Logic>>>    logicSystems;    //!< The list of logic systems.
    std::vector<std::unique_ptr<System<SystemType::Render>>>   renderSystems;   //!< The list of render systems.

    std::uint32_t               highestID = 1;  //!< Holds the highest id that hasn't yet been given to an entity (starts at 1).
    std::queue<std::uint32_t>   unusedIDs;      //!< Holds unused ids for entities that were removed.

    std::mutex nextFrameActionsAccess;                  
    std::queue<std::function<void()>> nextFrameActions; //!< Actions to be executed next frame.

    sf::RenderTarget*	target = nullptr;               //!< The render target for render systems, if any.
    KeyBinding*         keyBinding = nullptr;           //!< The keybinding for inputs, if any.
};

/**
 * Add several logic systems.
 * @tparam The systems to be added.
 */
template <typename... System>
void GameManager::addLogicSystems()
{
    (addLogicSystem(std::make_unique<System>(this)), ...);
}

/**
 * Add several render systems.
 * @tparam The systems to be added.
 */
template <typename... System>
void GameManager::addRenderSystems()
{
    (addRenderSystem(std::make_unique<System>(this)), ...);
}