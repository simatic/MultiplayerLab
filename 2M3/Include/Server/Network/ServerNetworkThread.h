#pragma once
#include <thread>
#include <Network/Buffer.h>
#include "ServerNetworkHandler.h"

class ServerNetworkThread {
private:
    std::thread backingThread{};
    ServerNetworkHandler& serverNetwork;
    sf::UdpSocket& socket;
    Buffer<Packet>& outputBuffer;
    bool ready = false;

public:
    explicit ServerNetworkThread(ServerNetworkHandler& handler, Buffer<Packet>& outputBuffer);
    ~ServerNetworkThread();

    /// Is the server ready to receive clients?
    bool isReady() const;

private:
    void threadCode();
};