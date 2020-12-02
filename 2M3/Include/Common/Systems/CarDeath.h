#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Health.h"

class CarDeath : public LogicSystem<Health>
{
public:
    void update(const sf::Time& dt) override;
};