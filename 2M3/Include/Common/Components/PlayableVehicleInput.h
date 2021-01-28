#pragma once
#include "Common/Components/Component.h"
#include "VehicleInput.h"

struct PlayableVehicleInput : public IdentifiableComponent<PlayableVehicleInput>
{
    PlayableVehicleInput() = default;
    PlayableVehicleInput(VehicleInput* inputs);
    PlayableVehicleInput(const PlayableVehicleInput& toCopy);

    VehicleInput* inputs = nullptr;
};