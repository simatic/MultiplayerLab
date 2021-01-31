#include "Common/Systems/System.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/Collider.h"
#include "Common/Components/Health.h"

/**
 * @class VehicleCollisionSystem
 * Handles collisions between vehicles and other entities.
 */
class VehicleCollisionSystem : public LogicSystem<Kinematics, Collider, Health>
{
public:
    explicit VehicleCollisionSystem(GameManager* const gameManager);

    void update(const sf::Time& dt) override;
};