#pragma once
#include "Common/Components/Component.h"
#include "VehicleInput.h"

struct PlayableCarInput : public IdentifiableComponent<PlayableCarInput>
{
    PlayableCarInput() = default;
    PlayableCarInput(VehicleInput* inputs);
    PlayableCarInput(const PlayableCarInput& toCopy);

    VehicleInput* inputs = nullptr;
};