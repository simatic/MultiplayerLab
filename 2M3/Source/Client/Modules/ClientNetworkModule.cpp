#include <Client/Modules/ClientNetworkModule.h>

ClientNetworkModule::ClientNetworkModule(const std::string& host, const unsigned short port) :
	INetworkModule(host, port, std::make_unique<ClientNetworkHandler>()),
	thread(*network, buffer, host, remotePort)
{}