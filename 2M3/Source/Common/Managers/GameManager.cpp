#include "Common/Managers/GameManager.h"
#include <cstdlib>
#include <exception>

void GameManager::clearAll()
{
    logicSystems.clear();
    renderSystems.clear();
    entitiesToRemove.clear();
    entities.clear();
}

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

void GameManager::addEntityWithIDNextFrame(std::shared_ptr<Entity> entity, std::uint32_t id) {
    entitiesToAdd.emplace(id, entity);
}

void GameManager::removeEntity(Entity* entity)
{
    if (entities.find(entity->getID()) != entities.end())
    {
        removeFromSystemsLists(entity);
        unusedIDs.emplace(entity->getID());
        entities.erase(entity->getID());
    }
}

void GameManager::removeEntityNextFrame(Entity* entity)
{
    if (entities.find(entity->getID()) != entities.end())
    {
        entitiesToRemove.emplace(entity);
    }
}

void GameManager::applyEntitiesToRemove()
{
    for (Entity* entity: entitiesToRemove)
    {
        removeEntity(entity);
    }
    entitiesToRemove.clear();
}

void GameManager::applyEntitiesToAdd() {
    for(const auto& [id, entity] : entitiesToAdd) {
        addEntityWithID(entity, id);
    }
    entitiesToAdd.clear();
}

void GameManager::addLogicSystem(std::unique_ptr<System<SystemType::Logic>> system)
{
    logicSystems.push_back(std::move(system));
}

void GameManager::addRenderSystem(std::unique_ptr<System<SystemType::Render>> system)
{
    renderSystems.push_back(std::move(system));
}

void GameManager::updateSystemListsForAllEntities()
{
    for (auto&& [id, entity]: entities)
    {
        updateSystemLists(entity.get());
    }
}

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
}

void GameManager::update(const sf::Time& dt)
{
    {
        std::lock_guard lk(nextFrameActionsAccess);
        while(!nextFrameActions.empty()) {
            auto action = nextFrameActions.front();
            action();
            nextFrameActions.pop();
        }
    }

    for (const auto& system : logicSystems)
    { 
        system->update(dt); 
    }

    applyEntitiesToRemove();
    applyEntitiesToAdd();
}

void GameManager::render(const sf::Time& dt)
{
    for (const auto& renderer: renderSystems)
    { 
        renderer->update(dt); 
    }
}

sf::RenderTarget* GameManager::getRenderTarget() const
{
    return target;
}

void GameManager::setRenderTarget(sf::RenderTarget* target)
{
    this->target = target;
}

KeyBinding* GameManager::getKeyBinding() const
{
    return keyBinding;
}

void GameManager::setKeyBinding(KeyBinding* keys)
{
    keyBinding = keys;
}

std::shared_ptr<Entity> GameManager::getEntityWithID(std::uint32_t id) const {
    // not using operator[] to avoid filling the map with nullptrs
    auto location = entities.find(id);
    if(location == entities.end()) { // if not found, check in entities to add next frame
        auto locationInNewEntities = entitiesToAdd.find(id);
        if(locationInNewEntities != entitiesToAdd.end()) {
            return locationInNewEntities->second;
        }

        // no entity, return nullptr
        return nullptr;
    }
    return location->second;
}

std::vector<std::shared_ptr<Entity>> GameManager::getEntityList() const {
    std::vector<std::shared_ptr<Entity>> result{};
    for(const auto& [_, entity] : entities) {
        result.push_back(entity);
    }
    return result;
}

void GameManager::nextFrame(std::function<void()> actionToDo) {
    std::lock_guard lk(nextFrameActionsAccess);
    nextFrameActions.push(actionToDo);
}
