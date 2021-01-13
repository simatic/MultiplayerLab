#pragma once
#include <thread>
#include <string>

#include <Common/Network/Network.h>
#include <Common/Network/NetworkHandler.h>
#include <Common/Network/Buffer.h>

class ClientNetworkThread {
private:
    std::thread backingThread{};
    NetworkHandler& network;
    Buffer& outputBuffer;
    std::string host;
    unsigned short remotePort;

public:
    ClientNetworkThread(NetworkHandler& handler, Buffer& outputBuffer, const std::string& host, unsigned short port);
    ~ClientNetworkThread();

private:
    void threadCode();
    void send(sf::UdpSocket& socket, std::unique_ptr<Packet> packet, const sf::IpAddress& address, unsigned short port);
    bool processWaitingPackets(sf::UdpSocket &socket);
};