#pragma once
#include <thread>
#include "ServerNetworkHandler.h"

class ServerNetworkThread {
private:
    std::thread backingThread{};
    ServerNetworkHandler& serverNetwork;
    sf::UdpSocket& socket;

public:
    explicit ServerNetworkThread(ServerNetworkHandler& handler);
    ~ServerNetworkThread();

private:
    void threadCode();
};