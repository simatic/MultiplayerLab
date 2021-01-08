#pragma once

#include "Module.h"

#include <string>
#include <memory>

class INetworkModule : public IModule {
public:
	INetworkModule(const std::string& host, const unsigned short port, std::unique_ptr<NetworkHandler> network);

	ModuleID getID() const override { return ModuleID::Network; };

	template<typename Packet, typename... Args>
	std::unique_ptr<Packet> createAndSend(Args... args);

protected:
	std::string host;
	unsigned short remotePort;

	std::unique_ptr<NetworkHandler> network;
};

template<typename Packet, typename... Args>
std::unique_ptr<Packet> INetworkModule::createAndSend(Args... args) {
	auto packet = network->create<Packet>(args...);
	packet->realSend(network->getSocket(), sf::IpAddress(host), remotePort);
	return packet;
}