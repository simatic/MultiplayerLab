#pragma once
#include "Common/Components/Component.h"
#include "CarInput.h"

struct PlayableCarInput : public IdentifiableComponent<PlayableCarInput>
{
    PlayableCarInput() = default;
    PlayableCarInput(CarInput* inputs);
    PlayableCarInput(const PlayableCarInput& toCopy);

    CarInput* inputs;
};