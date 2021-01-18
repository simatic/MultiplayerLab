#pragma once

#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include <GameManager.h>
#include <Server/Modules/ServerNetworkModule.h>
#include "Common/Network/Network.h"
#include "ServerNetworkHandler.h"
#include "ServerNetworkThread.h"
#include "IServerEventsListener.h"

class Server: public IServerEventsListener {
private:
    GameManager game;
    std::thread gameThread;

public:
    explicit Server(const std::string& ip, unsigned short port);

    void run();
    void stop();
    bool isRunning();
    bool isReady();
    ServerNetworkHandler& getNetworkHandler();
    ServerNetworkModule& getNetworkModule();

    void onEvent(const UdpClient& client, NetworkEvent::Event event) override;

    ~Server();

private:
    void runGame();
};