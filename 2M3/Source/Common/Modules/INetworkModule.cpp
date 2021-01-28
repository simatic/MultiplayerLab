#include <Common/Modules/INetworkModule.h>

INetworkModule::INetworkModule(const std::string& host, const unsigned short port, std::unique_ptr<NetworkHandler> network) :
	host(host),
	remotePort(port),
	network(std::move(network))
{}

Buffer<Packet>& INetworkModule::getBuffer() {
	return buffer;
}

bool INetworkModule::isBufferEmpty() const {
	return buffer.empty();
}

const std::unique_ptr<NetworkHandler> &INetworkModule::getNetwork() const {
    return network;
}

