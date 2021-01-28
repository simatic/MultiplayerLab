#include "Common/Systems/System.h"
#include "Common/Components/VehicleInput.h"
#include "Common/Components/VehicleEngine.h"
#include "Common/Components/Gun.h"

class GunSystem : public LogicSystem<VehicleInput, VehicleEngine, Gun>
{
public:
	explicit GunSystem(GameManager* const gameManager);

	void update(const sf::Time& dt) override;;
};