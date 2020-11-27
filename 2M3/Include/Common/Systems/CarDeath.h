#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Health.h"
#include "OldEntity.h"

class CarDeath : public SignedSystem<Health>
{
public:
    static void update(OldEntity* entity);
};