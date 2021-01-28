#include "Common/Systems/GUISystem.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/CarEngine.h"

void GUISystem::update(const sf::Time dt, OldEntity* entity)
{
    GUI* gui = entity->getComponent<GUI>();

    if (gui->car != nullptr)
    {
        gui->mSpeedometer.setRotation(180.f + 180.f * (length(gui->car->getComponent<Kinematics>()->velocity) / gui->car->getComponent<VehicleEngine>()->maxSpeed));
    }
}