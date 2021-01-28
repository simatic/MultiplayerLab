#pragma once

#include "Module.h"

#include <Common/Network/Buffer.h>

#include <string>
#include <memory>

class INetworkModule : public IModule {
public:
	INetworkModule(const std::string& host, const unsigned short port, std::unique_ptr<NetworkHandler> network);

	ModuleID getID() const override { return ModuleID::Network; };

	template<typename Packet, typename... Args>
	std::unique_ptr<Packet> create(Args... args);

	template <typename Packet, typename... Args>
	void createAndSend(Args... args);

	template<class Type>
	Buffer<Type> extractPacketsOfType();

	Buffer<Packet>& getBuffer();
	bool isBufferEmpty() const;

    const std::unique_ptr<NetworkHandler> &getNetwork() const;

protected:
	std::string host;
	unsigned short remotePort;

	std::unique_ptr<NetworkHandler> network;

protected:
    Buffer<Packet> buffer;
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