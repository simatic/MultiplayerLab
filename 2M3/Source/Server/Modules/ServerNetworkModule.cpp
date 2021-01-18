#include <Common/Network/Constants.h>
#include "Server/Modules/ServerNetworkModule.h"

ServerNetworkModule::ServerNetworkModule(std::unique_ptr<ServerNetworkHandler> networkHandler):
    INetworkModule("localhost", DEFAULT_PORT, std::move(networkHandler)){
    auto handler = dynamic_cast<ServerNetworkHandler*>(network.get());
    handler->setNetworkModule(this);
}

sf::Uint32 ServerNetworkModule::getNewNetworkID() {
    return networkID++;
}