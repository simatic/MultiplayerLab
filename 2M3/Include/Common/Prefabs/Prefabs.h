#pragma once

#include <SFML/System.hpp>
#include "Common/Entity.h"

/**
 * Shorthands to generate an entity type quickly (think of prefab as pre-fabricated)
 */
namespace Prefab {

    /**
     * Supported prefab types
     */
    enum class Type {
        Car, PlayableCar, Bullet, Wall
    };

    /**
     * Create a car
     * @param renderable should this entity have rendering components?
     * @return the newly created entity. This does NOT add it to the game
     */
    std::shared_ptr<Entity> createCar(const bool renderable);

    /**
     * Create a playable car (ie controllable by the client)
     * @param renderable should this entity have rendering components?
     * @return the newly created entity. This does NOT add it to the game
     */
    std::shared_ptr<Entity> createPlayableCar(const bool renderable);

    /**
     * Create a bullet
     * @param renderable should this entity have rendering components?
     * @return the newly created entity. This does NOT add it to the game
     */
    std::shared_ptr<Entity> createBullet(const bool renderable);

    /**
     * Create a wall
     * @param renderable should this entity have rendering components?
     * @return the newly created entity. This does NOT add it to the game
     */
    std::shared_ptr<Entity> createWall(const bool renderable);

    /**
     * Create a generic entity. Will call the corresponding create* method based on the `type` parameter
     * @param type which entity type to create
     * @param renderable should this entity have rendering components?
     * @return the newly created entity. This does NOT add it to the game
     */
    std::shared_ptr<Entity> create(Prefab::Type type, const bool renderable);
}
