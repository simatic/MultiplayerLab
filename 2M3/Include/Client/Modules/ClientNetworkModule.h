#pragma once

#include <Common/Modules/INetworkModule.h>

#include <Client/ClientNetworkHandler.h>
#include <Client/ClientNetworkThread.h>

#include <string>
#include <memory>
#include <State.h>

class ClientNetworkModule : public INetworkModule {
public:
	ClientNetworkModule(State::Context context, const std::string& host, const unsigned short port);

	State::Context& getClientContext();
	std::uint32_t getSelfEntityID() const;

	void setSelfEntityID(std::uint32_t entityID);

private:
    State::Context clientContext;
	ClientNetworkThread thread;
	std::uint32_t selfEntityID = std::numeric_limits<uint32_t>::max();
};