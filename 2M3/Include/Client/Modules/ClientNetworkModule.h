#pragma once

#include <Common/Modules/INetworkModule.h>

#include <Client/ClientNetworkHandler.h>
#include <Client/ClientNetworkThread.h>

#include <string>
#include <memory>
#include <Common/States/State.h>

/**
 * @class ClientNetworkModule
 * INetworkModule specialization for the client.
 */
class ClientNetworkModule : public INetworkModule {
public:
	ClientNetworkModule(State::Context context, const std::string& host, const unsigned short port);

	/**
	 * @return the Game Context.
	 */
	State::Context& getClientContext();

	/**
	 * @return the ID of the vehicle controlled by the player.
	 */
	std::uint32_t getSelfEntityID() const;

	/**
	 * Sets the ID of the vehicle controlled by the player.
	 */
	void setSelfEntityID(std::uint32_t entityID);

private:
    State::Context clientContext;	//!< Game context.
	ClientNetworkThread thread;		//!< Thread to handle packet reception.
	std::uint32_t selfEntityID = std::numeric_limits<uint32_t>::max();	//!< ID of the vehicle controlled by the player.
};