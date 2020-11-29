#include "Common/Systems/System.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/Collider.h"
#include "Common/Components/Health.h"

class CarCollisionHandling : public SignedSystem<Kinematics, Collider, Health>
{
public:
    void update(const sf::Time& dt) override;
};