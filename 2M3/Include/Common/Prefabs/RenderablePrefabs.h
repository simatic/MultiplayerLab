#pragma once
#include "Common/Entity.h"

namespace Prefab {

    std::shared_ptr<Entity> createRenderableCar();
    std::shared_ptr<Entity> createPlayableCar();
}