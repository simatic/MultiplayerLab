#pragma once
#include <thread>
#include "ServerNetworkHandler.h"

class ServerNetworkThread {
private:
    std::thread backingThread{};
    ServerNetworkHandler& serverNetwork;
    sf::UdpSocket& socket;
    bool ready = false;

public:
    explicit ServerNetworkThread(ServerNetworkHandler& handler);
    ~ServerNetworkThread();

    /// Is the server ready to receive clients?
    bool isReady() const;

private:
    void threadCode();
};