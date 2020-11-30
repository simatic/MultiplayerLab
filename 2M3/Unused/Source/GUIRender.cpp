#include "Common/Systems/GUIRender.h"

void GUIRender::update(const sf::Time dt, OldEntity* entity, sf::RenderTarget& target, sf::RenderStates states)
{
    GUI* gui = entity->getComponent<GUI>();

    if (gui->car != nullptr)
    {
        target.draw(gui->mSpeedometerBackground, states);
        target.draw(gui->mSpeedometer, states);
    }
}