#pragma once

#include "Module.h"

#include <Common/Network/Buffer.h>

#include <string>
#include <memory>

class INetworkModule : public IModule {
public:
	INetworkModule(const std::string& host, const unsigned short port, std::unique_ptr<NetworkHandler> network);

	ModuleID getID() const override { return ModuleID::Network; };

	template <typename Packet, typename... Args>
	std::unique_ptr<Packet> createAndSend(Args... args);

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

template<typename Packet, typename... Args>
std::unique_ptr<Packet> INetworkModule::createAndSend(Args... args) {
	auto packet = network->create<Packet>(args...);
	packet->realSend(network->getSocket(), sf::IpAddress(host), remotePort);
	return packet;
}