#include <Common/Modules/INetworkModule.h>

INetworkModule::INetworkModule(const std::string& host, const unsigned short port, std::unique_ptr<NetworkHandler> network) :
	host(host),
	remotePort(port),
	network(std::move(network))
{}

Buffer& INetworkModule::getBuffer() {
	return buffer;
}