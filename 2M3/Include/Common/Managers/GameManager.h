#pragma once
#include "Common/Systems/System.h"
#include "Common/Entity.h"
#include "KeyBinding.h"

#include <memory>
#include <vector>
#include <queue>

#include <SFML/Graphics/RenderTarget.hpp>

class GameManager
{
public:
    /**
     * Singleton management
     */
    static GameManager* getInstance()
    {
        if (instance == 0)
        {
            instance = new GameManager();
        }

        return instance;
    }

    /**
     * Methods
     */
    void addEntity(std::shared_ptr<Entity> entity);
    void removeEntityNextFrame(Entity* entity);
    void removeEntity(Entity* entity);

    void addLogicSystem(std::unique_ptr<System<SystemType::Logic>> system);
    void addRenderSystem(std::unique_ptr<System<SystemType::Render>> renderer);

    void update(const sf::Time& dt);
    void render(const sf::Time& dt);

    void updateSystemLists();
    void updateSystemLists(Entity* entity);

    sf::RenderTarget* getRenderTarget() const;
    void              setRenderTarget(sf::RenderTarget* target);

    KeyBinding*       getKeyBinding() const;
    void              setKeyBinding(KeyBinding* keys);

    void clearAll();

    // templates
    template<typename... System>
    void addLogicSystems();

    template<typename... System>
    void addRenderSystems();

private:
    /**
     * Singleton management
     */
    GameManager();
    ~GameManager() = default;

    GameManager(const GameManager&);
    GameManager& operator=(const GameManager&);

    static void cleanUp();

    static GameManager* instance;

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

    std::uint32_t               highestID = 1;
    std::queue<std::uint32_t>   unusedIDs;

    sf::RenderTarget*	target = nullptr;
    KeyBinding*         keyBinding = nullptr;
};

template<typename... System>
void GameManager::addLogicSystems() {
    (
        addLogicSystem(std::make_unique<System>())
    , ...);
}

template<typename... System>
void GameManager::addRenderSystems() {
    (
        addRenderSystem(std::make_unique<System>())
    , ...);
}