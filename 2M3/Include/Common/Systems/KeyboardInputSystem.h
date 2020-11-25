#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/CarInput.h"
#include "OldEntity.h"
#include "KeyBinding.h"

class KeyboardInputSystem : public SignedSystem<CarInput>
{
public:
	static void update(OldEntity* entity, KeyBinding* keyBindings);
};