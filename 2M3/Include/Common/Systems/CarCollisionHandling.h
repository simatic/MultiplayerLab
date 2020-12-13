#include "Common/Systems/System.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/Collider.h"
#include "Common/Components/Health.h"

class CarCollisionSystem : public LogicSystem<Kinematics, Collider, Health>
{
public:
    explicit CarCollisionSystem(GameManager* const gameManager);

    void update(const sf::Time& dt) override;
};