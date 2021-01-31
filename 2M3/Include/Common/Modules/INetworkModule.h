#pragma once

#include "Module.h"

#include <Common/Network/Buffer.h>

#include <string>
#include <memory>

/**
 * @class INetworkModule
 * Abstract base class for client and server network modules.
 */
class INetworkModule : public IModule {
public:
	INetworkModule(const std::string& host, const unsigned short port, std::unique_ptr<NetworkHandler> network);

	ModuleID getID() const override { return ModuleID::Network; };

	/**
	 * Creates a network packet.
	 * @tparam The packet to create.
	 * @return the created packet.
	 */
	template<typename Packet, typename... Args>
	std::unique_ptr<Packet> create(Args... args);

	/**
	 * Creates a network packet and send it.
	 * @tparam The packet to create.
	 */
	template <typename Packet, typename... Args>
	void createAndSend(Args... args);

	/**
	 * Sorts recieved packets by type and returns a buffer with only the packets of a certain type.
	 * @tparam Type of the packets to retrieve.
	 */
	template<class Type>
	Buffer<Type> extractPacketsOfType();

	/**
	 * @return the Buffer.
	 */
	Buffer<Packet>& getBuffer();

	/**
	 * If the buffer is empty.
	 */
	bool isBufferEmpty() const;

	/**
	 * @return the Network Handler.
	 */
    const std::unique_ptr<NetworkHandler> &getNetwork() const;

protected:
	std::string host;			//!< Host, i.e. server address.
	unsigned short remotePort;	//!< Port on which to communicate.

	std::unique_ptr<NetworkHandler> network;	//!< Associated network handler.

protected:
    Buffer<Packet> buffer;	//!< Buffer for incoming packets.
};

template<class Type>
Buffer<Type> INetworkModule::extractPacketsOfType() {
	return buffer.extractPacketsOfType<Type>();
}

template<typename Packet, typename... Args>
std::unique_ptr<Packet> INetworkModule::create(Args... args) {
	return network->create<Packet>(args...);
}

template<typename Packet, typename... Args>
void INetworkModule::createAndSend(Args... args) {
	network->create<Packet>(args...)->realSend(network->getSocket(), sf::IpAddress(host), remotePort);
}