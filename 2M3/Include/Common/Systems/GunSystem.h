#include "Common/Systems/System.h"
#include "Common/Components/CarInput.h"
#include "Common/Components/CarEngine.h"
#include "Common/Components/Gun.h"

class GunSystem : public LogicSystem<CarInput, CarEngine, Gun>
{
public:
	explicit GunSystem(GameManager* const gameManager);

	void update(const sf::Time& dt) override;;
};