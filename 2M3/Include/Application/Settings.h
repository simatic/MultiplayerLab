#pragma once
#include <Common/Resources/KeyBinding.h>

struct Settings
{
	Settings() = default;

	bool changed = false;
	KeyBinding playerAKeyBinding{ 2 };
	KeyBinding playerBKeyBinding{ 1 };
};