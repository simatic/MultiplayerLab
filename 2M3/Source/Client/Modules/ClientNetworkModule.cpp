#include <Client/Modules/ClientNetworkModule.h>

ClientNetworkModule::ClientNetworkModule(State::Context context, const std::string& host, const unsigned short port) :
	INetworkModule(host, port, std::make_unique<ClientNetworkHandler>()),
	thread(*network, buffer, host, remotePort),
	clientContext(context)
{
    thread.setNetworkModule(this);
}

State::Context& ClientNetworkModule::getClientContext() {
    return clientContext;
}

std::uint32_t ClientNetworkModule::getSelfEntityID() const {
    return selfEntityID;
}

void ClientNetworkModule::setSelfEntityID(std::uint32_t entityID) {
    selfEntityID = entityID;
}
