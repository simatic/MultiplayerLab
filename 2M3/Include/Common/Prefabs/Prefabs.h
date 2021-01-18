#pragma once
#include "Common/Entity.h"

namespace Prefab {
    enum Type{car, playableCar, bullet};

    std::shared_ptr<Entity> createCar(const bool renderable);
    std::shared_ptr<Entity> createPlayableCar(const bool renderable);
    std::shared_ptr<Entity> createBullet(const bool renderable);
}