#pragma once
#include <Common/Entity.h>
#include <Common/Systems/System.h>
#include <Common/Resources/KeyBinding.h>
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
     * Add entity.
     * @param entity Entity to add.
     */
    void addEntity(std::shared_ptr<Entity> entity);

    /**
     * Add entity with a specific id.
     * @param entity Entity to add.
     * @param id The specific id.
     */
    void addEntityWithID(std::shared_ptr<Entity> entity, std::uint32_t id);

    /**
     * Add entity with a specific id on the next frame.
     * @param entity Entity to add.
     * @param id The specific id.
     */
    void addEntityWithIDNextFrame(std::shared_ptr<Entity> entity, std::uint32_t id);

    /**
     * Obtain a sepcific entity by id.
     * @param id
     */
    std::shared_ptr<Entity> getEntityWithID(std::uint32_t id) const;

    /**
     * Set the given entity to be deleted on next frame.
     * This is safe to use while the game loop is running.
     */
    void removeEntityNextFrame(Entity* entity);

    /**
     * Removes and deletes the entity.
     * @warning Do not use while game loop is running. Use removeEntityNextFrame() instead.
     */
    void removeEntity(Entity* entity);

    /**
     * Update logic systems.
     * @param dt Time since last frame.
     */
    virtual void update(const sf::Time& dt);

    /**
     * Update registered renderers.
     * @param dt Time since last frame.
     */
    void render(const sf::Time& dt);

    /**
     * Add system. Ownership is transferred to GameManager.
     * @param system System to add.
     */
    void addLogicSystem(std::unique_ptr<System<SystemType::Logic>> system);

    /**
     * Add renderer. Ownership is transferred to GameManager.
     * @param system System to add.
     */
    void addRenderSystem(std::unique_ptr<System<SystemType::Render>> system);

    /**
     * Assign entities to their systems.
     */
    virtual void updateSystemListsForAllEntities();

    /**
     * Assign an entity to its systems.
     * @param entity Entity to assign.
     */
    virtual void updateSystemLists(Entity* entity);

    /**
     * @return The target used to draw on.
     */
    sf::RenderTarget* getRenderTarget() const;
    /**
     * @param target The new target used to draw on.
     */
    void              setRenderTarget(sf::RenderTarget* target);

    /**
     * @return Keyboard key binding.
     */
    KeyBinding*       getKeyBinding() const;
    /**
     * @param keys The key binding.
     */
    void              setKeyBinding(KeyBinding* keys);

    /**
     * Deletes all entities and systems.
     */
    virtual void clearAll();

    /**
     * Add several logic systems.
     * @tparam The systems to be added.
     */
    template <typename... System>
    void addLogicSystems();

    /**
     * Add several render systems.
     * @tparam The systems to be added.
     */
    template <typename... System>
    void addRenderSystems();

    /**
     * Add a new action to do on the next frame.
     */
    void nextFrame(std::function<void()> actionToDo);

    /**
     * Obtain the list of entities.
     */
    std::vector<std::shared_ptr<Entity>> getEntityList() const;

protected:
    /**
     * Removes and deletes all entities in entitiesToRemove.
     */
    void applyEntitiesToRemove();

    /**
     * Adds all entities in entitiesToAdd.
     */
    void applyEntitiesToAdd();

    /**
     * Add system. Ownership is transferred to GameManager.
     * @param system System to add.
     */
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