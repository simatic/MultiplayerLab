#include <Common/Network/Constants.h>
#include "Server/Modules/ServerNetworkModule.h"

ServerNetworkModule::ServerNetworkModule(const std::string ip, unsigned short port):
    INetworkModule("localhost", DEFAULT_PORT, std::move(std::make_unique<ServerNetworkHandler>(ip, port))),
    networkThread(*dynamic_cast<ServerNetworkHandler*>(this->network.get())){
    auto handler = dynamic_cast<ServerNetworkHandler*>(network.get());
    handler->setNetworkModule(this);
}

sf::Uint32 ServerNetworkModule::getNewNetworkID() {
    return networkID++;
}

const ServerNetworkThread& ServerNetworkModule::getNetworkThread() const {
    return networkThread;
}