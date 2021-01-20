#include <Common/Network/Constants.h>
#include "Server/Modules/ServerNetworkModule.h"

ServerNetworkModule::ServerNetworkModule(Server& server, const std::string ip, unsigned short port):
    INetworkModule("localhost", DEFAULT_PORT, std::move(std::make_unique<ServerNetworkHandler>(buffer, ip, port))),
    networkThread(*dynamic_cast<ServerNetworkHandler*>(this->network.get()), buffer),
    server(server) {
    auto handler = dynamic_cast<ServerNetworkHandler*>(network.get());
    handler->setNetworkModule(this);
}

sf::Uint32 ServerNetworkModule::getNewNetworkID() {
    return networkID++;
}

const ServerNetworkThread& ServerNetworkModule::getNetworkThread() const {
    return networkThread;
}

Server& ServerNetworkModule::getServer() {
    return server;
}
