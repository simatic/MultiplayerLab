#include "PlayableVehicleInput.h"

PlayableVehicleInput::PlayableVehicleInput(const PlayableVehicleInput& toCopy): inputs(toCopy.inputs) {}

PlayableVehicleInput::PlayableVehicleInput(VehicleInput* inputs): inputs(inputs) {}