#pragma once

#include <Common/Modules/INetworkModule.h>

#include <Client/ClientNetworkHandler.h>
#include <Client/ClientNetworkThread.h>

#include <string>
#include <memory>

class ClientNetworkModule : public INetworkModule {
public:
	ClientNetworkModule(const std::string& host, const unsigned short port);

private:
	ClientNetworkThread thread;
};