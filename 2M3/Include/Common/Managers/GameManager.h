#pragma once
#include "Common/Systems/System.h"
#include "Common/Entity.h"
#include "KeyBinding.h"

#include <memory>
#include <vector>
#include <queue>

#include <SFML/Graphics/RenderTarget.hpp>

class Entity;
class System;

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
    void removeEntityNextFrame(std::uint32_t id);
    void removeEntity(std::uint32_t id);

    void setPlayer(Entity* entity);
    Entity* getPlayer() const;

    void addSystem(std::unique_ptr<System> system);
    void addRenderer(std::unique_ptr<System> renderer);

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
    void addSystems();

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

    /**
     * Attributes
     */
    Entity* player;
    std::unordered_map<std::uint32_t, std::shared_ptr<Entity>>  entities;
    std::set<std::uint32_t>                 entitiesToRemove;

    std::vector<std::unique_ptr<System>>    systems;
    std::vector<std::unique_ptr<System>>    renderers;

    std::uint32_t               highestID = 1;
    std::queue<std::uint32_t>   unusedIDs;

    sf::RenderTarget*	target;
    KeyBinding*         keyBinding;
};

template<typename... System>
void GameManager::addSystems() {
    (
            addSystem(std::make_unique<System>())
    , ...);
}