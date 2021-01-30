#pragma once
#include "Common/Components/Component.h"
#include "VehicleInput.h"

/**
 * @struct PlayableVehicleInput component.
 * Specific component to identify the entity which is directly controlled by the user.
 */
struct PlayableVehicleInput : public IdentifiableComponent<PlayableVehicleInput>
{
    PlayableVehicleInput() = default;
    PlayableVehicleInput(VehicleInput* inputs);
    PlayableVehicleInput(const PlayableVehicleInput& toCopy);

    VehicleInput* inputs = nullptr;
};