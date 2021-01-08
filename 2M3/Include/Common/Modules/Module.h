#pragma once

#include <Common/Network/Network.h>
#include <Common/Network/NetworkHandler.h>

#include <string>
#include <memory>

enum class ModuleID {
	Network
};

class IModule {
public:
	virtual ModuleID getID() const = 0;
};