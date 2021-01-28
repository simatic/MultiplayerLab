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
	std::unique_ptr<Packet> createAndSend(Args... args);

	template<class Type>
	std::queue<std::unique_ptr<Type>> extractPacketsOfType();

	Buffer& getBuffer();
	bool isBufferEmpty() const;

    const std::unique_ptr<NetworkHandler> &getNetwork() const;

protected:
	std::string host;
	unsigned short remotePort;

	std::unique_ptr<NetworkHandler> network;

protected:
    Buffer buffer;
};

template<class Type>
std::queue<std::unique_ptr<Type>> INetworkModule::extractPacketsOfType() {
	return buffer.extractPacketsOfType<Type>();
}

template<typename Packet, typename... Args>
std::unique_ptr<Packet> INetworkModule::create(Args... args) {
	return network->create<Packet>(args...);
}

template<typename Packet, typename... Args>
std::unique_ptr<Packet> INetworkModule::createAndSend(Args... args) {
	auto packet = network->create<Packet>(args...);
	packet->realSend(network->getSocket(), sf::IpAddress(host), remotePort);
	return packet;
}