#include "PlayableCarInput.h"

PlayableCarInput::PlayableCarInput(const PlayableCarInput& toCopy): inputs(toCopy.inputs) {}

PlayableCarInput::PlayableCarInput(VehicleInput* inputs): inputs(inputs) {}