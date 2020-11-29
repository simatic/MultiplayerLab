#include "Common/Systems/System.h"
#include "Common/Components/CarInput.h"
#include "Common/Components/CarEngine.h"
#include "Common/Components/Gun.h"

class GunSystem : public SignedSystem<CarInput, CarEngine, Gun>
{
public:
	void update(const sf::Time& dt) override;;
};