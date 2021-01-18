#include "Common/Managers/GameManager.h"
#include <cstdlib>
#include <exception>

/**
 * Deletes all entities and systems.
 */
void GameManager::clearAll()
{
    entities.clear();
    logicSystems.clear();
    renderSystems.clear();
    networkSystems.clear();
    entitiesToRemove.clear();
}

/**
 * Add entity.
 * @param entity Entity to add.
 */
void GameManager::addEntity(std::shared_ptr<Entity> entity)
{
    uint32_t id = 0;
    if (unusedIDs.size() != 0)
    {
        id = unusedIDs.front();
        unusedIDs.pop();
    }
    else
    {
        id = highestID++;
    }

    addEntityWithID(entity, id);
}

void GameManager::addEntityWithID(std::shared_ptr<Entity> entity, std::uint32_t id) {
    entity->setID(id);
    entities.emplace(entity->getID(), entity);
    updateSystemLists(entity.get());
}

/**
 * Removes and deletes the entity.
 * @warning Do not use while game loop is running. Use removeEntityNextFrame() instead.
 */
void GameManager::removeEntity(Entity* entity)
{
    if (entities.find(entity->getID()) != entities.end())
    {
        removeFromSystemsLists(entity);
        unusedIDs.emplace(entity->getID());
        entities.erase(entity->getID());
    }
}

/**
 * Set the given entity to be deleted on next frame.
 * This is safe to use while the game loop is running.
 */
void GameManager::removeEntityNextFrame(Entity* entity)
{
    if (entities.find(entity->getID()) != entities.end())
    {
        entitiesToRemove.emplace(entity);
    }
}

/**
 * Removes and deletes all entities in entitiesToRemove.
 */
void GameManager::applyEntitiesToRemove()
{
    for (Entity* entity: entitiesToRemove)
    {
        removeEntity(entity);
    }
    entitiesToRemove.clear();
}

/**
 * Add system. Ownership is transferred to GameManager.
 * @param system System to add.
 */
void GameManager::addLogicSystem(std::unique_ptr<System<SystemType::Logic>> system)
{
    logicSystems.push_back(std::move(system));
}

/**
 * Add renderer. Ownership is transferred to GameManager.
 * @param system System to add.
 */
void GameManager::addRenderSystem(std::unique_ptr<System<SystemType::Render>> system)
{
    renderSystems.push_back(std::move(system));
}

/**
 * Add renderer. Ownership is transferred to GameManager.
 * @param system System to add.
 */
void GameManager::addNetworkSystem(std::unique_ptr<System<SystemType::Network>> system)
{
    networkSystems.push_back(std::move(system));
}

/**
 * Assign entities to their systems.
 */
void GameManager::updateSystemLists()
{
    for (auto&& [id, entity]: entities)
    {
        updateSystemLists(entity.get());
    }
}

/**
 * Assign an entity to its systems.
 * @param entity Entity to assign.
 */
void GameManager::updateSystemLists(Entity* entity)
{
    for (auto& system: logicSystems)
    {
        if ((entity->getSignature() & system->getSignature()) == system->getSignature())
        {
            system->addEntity(entity);
        }
        else
        {
            system->removeEntity(entity);
        }
    }

    for (auto& system: renderSystems)
    {
        if ((entity->getSignature() & system->getSignature()) == system->getSignature())
        {
            system->addEntity(entity);
        }
        else
        {
            system->removeEntity(entity);
        }
    }

    for (auto& system : networkSystems)
    {
        if ((entity->getSignature() & system->getSignature()) == system->getSignature())
        {
            system->addEntity(entity);
        }
        else
        {
            system->removeEntity(entity);
        }
    }
}

void GameManager::removeFromSystemsLists(Entity* entity) 
{
    for (auto& system : logicSystems)
    {
        system->removeEntity(entity);
    }

    for (auto& system : renderSystems)
    {
        system->removeEntity(entity);
    }

    for (auto& system : networkSystems)
    {
        system->removeEntity(entity);
    }
}

/**
 * Update logic systems.
 * @param dt Time since last frame.
 */
void GameManager::update(const sf::Time& dt)
{
    for (std::unique_ptr<System<SystemType::Logic>>& system: logicSystems)
    { 
        system->update(dt); 
    }

    for (std::unique_ptr<System<SystemType::Network>>& system : networkSystems)
    {
        system->update(dt);
    }
    applyEntitiesToRemove();
}

/**
 * Update registered renderers.
 * @param dt Time since last frame.
 */
void GameManager::render(const sf::Time& dt)
{
    for (std::unique_ptr<System<SystemType::Render>>& renderer: renderSystems)
    { 
        renderer->update(dt); 
    }
}

/**
 * @return The target used to draw on.
 */
sf::RenderTarget* GameManager::getRenderTarget() const
{
    return target;
}

/**
 * @param target The new target used to draw on.
 */
void GameManager::setRenderTarget(sf::RenderTarget* target)
{
    this->target = target;
}

/**
 * @return Keyboard key binding.
 */
KeyBinding* GameManager::getKeyBinding() const
{
    return keyBinding;
}

/**
 * @param keys The key binding.
 */
void GameManager::setKeyBinding(KeyBinding* keys)
{
    keyBinding = keys;
}

INetworkModule* GameManager::getNetworkModule() const
{
    if (networkModule == nullptr)
    {
        throw std::runtime_error("Network module hasn't been initialized.");
    }
    return networkModule.get();
}