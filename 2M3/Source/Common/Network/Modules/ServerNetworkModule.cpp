#include <Server/Modules/ServerNetworkModule.h>
#include "../../Common/Network/Constants.h"

ServerNetworkModule::ServerNetworkModule(const std::string& host, const unsigned short port, std::unique_ptr<ServerNetworkHandler> networkHandler) :
        INetworkModule(host, port, std::move(networkHandler))
{
    networkID = 0;
}

sf::Uint32 ServerNetworkModule::getNewNetworkID() {
    networkID++;
    return networkID;
}

