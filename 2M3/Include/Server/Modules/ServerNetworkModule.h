#pragma once

#include <Common/Modules/INetworkModule.h>

#include <Server/ServerNetworkHandler.h>
#include <Server/ServerNetworkThread.h>

#include <string>
#include <memory>

class Server;

class ServerNetworkModule : public INetworkModule {
private:
    //Entity ID -> networkID in AddEntityPacket.h
    sf::Uint32 networkID = 0;
    ServerNetworkThread networkThread;
    Server& server;

public:
    ServerNetworkModule(Server& server, const std::string ip, unsigned short port);

    sf::Uint32 getNewNetworkID();

    const ServerNetworkThread& getNetworkThread() const;

    Server& getServer();
};