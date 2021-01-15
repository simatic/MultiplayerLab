#pragma once

#include <Common/Modules/INetworkModule.h>

#include <Server/ServerNetworkHandler.h>
#include <Server/ServerNetworkThread.h>

#include <string>
#include <memory>

class ServerNetworkModule : public INetworkModule {
private:
    //Entity ID -> networkID in AddEntityPacket.h
    sf::Uint32 networkID = 0;

public:
    ServerNetworkModule(std::unique_ptr<ServerNetworkHandler> networkHandler);

    sf::Uint32 getNewNetworkID();
};