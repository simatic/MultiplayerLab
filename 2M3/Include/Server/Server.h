#pragma once

#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include <Server/Modules/ServerNetworkModule.h>
#include "Common/Network/Network.h"
#include "ServerNetworkHandler.h"
#include "ServerNetworkThread.h"

class Server {
private:
    ServerNetworkModule networkModule;

public:
    explicit Server(const std::string& ip, unsigned short port);

    void run();
    void stop();
    bool isRunning();

    ~Server();
};