#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Health.h"

/**
 * @class VehicleDeath
 * Handles vehicle life.
 */
class VehicleDeath : public LogicSystem<Health>
{
public:
    explicit VehicleDeath(GameManager* const gameManager);

    void update(const sf::Time& dt) override;
};