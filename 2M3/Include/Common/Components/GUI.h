#include "Common/Components/Component.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "Common/Entity.h"

struct GUI : public IdentifiableComponent<GUI>
{
    GUI(Entity* car);

    Entity* car;
    sf::RectangleShape mSpeedometer;
	sf::CircleShape mSpeedometerBackground;
};
