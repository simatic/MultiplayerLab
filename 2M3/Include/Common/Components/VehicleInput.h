#pragma once
#include "Common/Components/Component.h"

/**
 * @struct PlayableVehicleInput component.
 * Store inputs to control a vehicle.
 */
struct VehicleInput : public IdentifiableComponent<VehicleInput>
{
    VehicleInput() = default;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool action = false;
    bool changeActionEvent = false;
    bool doActionEvent = false;
};