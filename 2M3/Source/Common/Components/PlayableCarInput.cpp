#include "PlayableCarInput.h"

PlayableCarInput::PlayableCarInput(const PlayableCarInput& toCopy): inputs(toCopy.inputs) {}

PlayableCarInput::PlayableCarInput(CarInput* inputs): inputs(inputs) {}