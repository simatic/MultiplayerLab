#include "Common/Systems/System.h"
#include "Common/Components/CarInput.h"
#include "Common/Components/VehicleEngine.h"
#include "Common/Components/Gun.h"

class GunSystem : public LogicSystem<CarInput, VehicleEngine, Gun>
{
public:
	explicit GunSystem(GameManager* const gameManager);

	void update(const sf::Time& dt) override;;
};