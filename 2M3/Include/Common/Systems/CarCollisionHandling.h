#include "Common/Systems/System.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/Collider.h"
#include "Common/Components/Health.h"
#include "OldEntity.h"

class CarCollisionHandling : public SignedSystem<Kinematics, Collider, Health>
{
public:
    static void update(OldEntity* entity);
};