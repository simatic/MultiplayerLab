#pragma once
#include "Common/Components/Component.h"

struct CarInput : public IdentifiableComponent<CarInput>
{
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool action = false;
    bool changeActionEvent = false;
    bool doActionEvent = false;
};