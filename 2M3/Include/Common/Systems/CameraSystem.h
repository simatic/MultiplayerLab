#pragma once

#include "System.h"
#include "Common/Components/CameraTarget.h"
#include "Common/Components/Transform.h"

class CameraSystem: public SignedSystem<Transform, CameraTarget> {
public:
    explicit CameraSystem();

    void update(const sf::Time& dt) override;
};
