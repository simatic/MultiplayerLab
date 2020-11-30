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
    void addEntity(std::unique_ptr<Entity> entity);
    void removeEntityNextFrame(std::uint32_t id);
    void removeEntity(std::uint32_t id);

    void addSystem(std::unique_ptr<System> system);
    void addRenderer(std::unique_ptr<System> renderer);

    void update(const sf::Time& dt) { for (std::unique_ptr<System>& system: systems) { system->update(dt); }}
    void render(const sf::Time& dt) { for (std::unique_ptr<System>& system: renderers) { system->update(dt); }}

    void updateSystemLists();
    void updateSystemLists(Entity* entity);

    sf::RenderTarget* getRenderTarget() const { return target; };
    void setRenderTarget(sf::RenderTarget* target) { this->target = target; };

    KeyBinding* getKeyBinding() const { return keyBinding; };
    void setKeyBinding(KeyBinding* keys) { this->keyBinding = keys; };

    void clearAll();

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
    std::unordered_map<std::uint32_t, std::unique_ptr<Entity>>  entities;
    std::set<std::uint32_t>                 entitiesToRemove;

    std::vector<std::unique_ptr<System>>    systems;
    std::vector<std::unique_ptr<System>>    renderers;

    std::uint32_t               highestID = 1;
    std::queue<std::uint32_t>   unusedIDs;

    sf::RenderTarget*	target;
    KeyBinding*         keyBinding;
};