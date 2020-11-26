#include "Common/Systems/System.h"
#include "Common/Components/CarInput.h"
#include "Common/Components/CarEngine.h"
#include "Common/Components/Gun.h"
#include "OldEntity.h"
#include "CarLogic.h"

class GunSystem : public SignedSystem<CarInput, CarEngine, Gun>
{
public:
	static void update(const sf::Time dt, CarLogic* entity, std::vector<OldEntity*>& newEntities);
};