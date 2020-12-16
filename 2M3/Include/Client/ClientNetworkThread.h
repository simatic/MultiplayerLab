#pragma once
#include <thread>
#include <string>

#include <Common/Network/Network.h>
#include <Common/Network/NetworkHandler.h>

class ClientNetworkThread {
private:
    std::thread backingThread{};
    NetworkHandler& network;
    std::string host;
    unsigned short remotePort;

public:
    ClientNetworkThread(NetworkHandler& handler, const std::string& host, unsigned short port);
    ~ClientNetworkThread();

private:
    void threadCode();
    void send(sf::UdpSocket& socket, std::unique_ptr<Packet> packet, const sf::IpAddress& address, unsigned short port);
    bool processWaitingPackets(sf::UdpSocket &socket);
};