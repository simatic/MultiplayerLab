#include "Common/Managers/GameManager.h"
#include <cstdlib>

GameManager* GameManager::instance = 0;

GameManager::GameManager()
{
    std::atexit(&cleanUp);
}

void GameManager::cleanUp()
{
    if (instance != 0)
        delete instance;
    instance = 0;
}

void GameManager::clearAll()
{
    entities.clear();
    systems.clear();
}

void GameManager::addEntity(std::unique_ptr<Entity> entity)
{
    if (unusedIDs.size() != 0)
    {
        entity->setID(unusedIDs.front());
        unusedIDs.pop();
    }
    else
    {
        entity->setID(highestID++);
    }

    entities.emplace(entity->getID(), std::move(entity));
}

void GameManager::removeEntity(std::uint32_t id)
{
    if (entities.find(id) != entities.end())
    {
        entities.erase(id);
        unusedIDs.emplace(id);
    }
}

void GameManager::removeEntityNextFrame(std::uint32_t id)
{
    if (entities.find(id) != entities.end())
    {
        entitiesToRemove.emplace(id);
    }
}

void GameManager::applyEntitiesToRemove()
{
    for (const std::uint32_t id: entitiesToRemove)
    {
        removeEntity(id);
    }
    entitiesToRemove.clear();
}

void GameManager::addSystem(std::unique_ptr<System> system)
{
    systems.push_back(std::move(system));
}

void GameManager::addRenderer(std::unique_ptr<System> system)
{
    renderers.push_back(std::move(system));
}

void GameManager::updateSystemLists()
{
    for (auto&& [id, entity]: entities)
    {
        updateSystemLists(entity.get());
    }
}

void GameManager::updateSystemLists(Entity* entity)
{
    for (std::unique_ptr<System>& system: systems)
    {
        if ((entity->getSignature() & system->signature) == system->signature)
        {
            system->addEntity(entity);
        }
        else
        {
            system->removeEntity(entity);
        }
    }

    for (std::unique_ptr<System>& system: renderers)
    {
        if ((entity->getSignature() & system->signature) == system->signature)
        {
            system->addEntity(entity);
        }
        else
        {
            system->removeEntity(entity);
        }
    }
}