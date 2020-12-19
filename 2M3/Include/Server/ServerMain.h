#pragma once

#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include "Common/Network/Network.h"
#include "ServerNetworkHandler.h"
#include "ServerNetworkThread.h"

class Server {
private:
    ServerNetworkHandler networkHandler;
    ServerNetworkThread networkThread;

public:
    explicit Server(const std::string& ip, unsigned short port);

    void run();

    ~Server();
};