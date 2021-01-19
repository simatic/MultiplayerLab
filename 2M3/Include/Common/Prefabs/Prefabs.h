#pragma once

#include <SFML/System.hpp>
#include "Common/Entity.h"

namespace Prefab {
    enum class Type {
        Car, PlayableCar, Bullet, Wall
    };

    std::shared_ptr<Entity> createCar(const bool renderable);
    std::shared_ptr<Entity> createPlayableCar(const bool renderable);
    std::shared_ptr<Entity> createBullet(const bool renderable);
    std::shared_ptr<Entity> createWall(const bool renderable);

    std::shared_ptr<Entity> create(Prefab::Type type, const bool renderable);
}
