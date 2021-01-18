#pragma once

#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include <GameManager.h>
#include <Server/Modules/ServerNetworkModule.h>
#include "Common/Network/Network.h"
#include "ServerNetworkHandler.h"
#include "ServerNetworkThread.h"

class Server {
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

    ~Server();

private:
    void runGame();
};