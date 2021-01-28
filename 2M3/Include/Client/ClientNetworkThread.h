#pragma once
#include <thread>
#include <string>

#include <Common/Network/Network.h>
#include <Common/Network/NetworkHandler.h>
#include <Common/Network/Buffer.h>

class ClientNetworkModule;

class ClientNetworkThread {
private:
    std::thread backingThread{};
    NetworkHandler& network;
    Buffer<Packet>& outputBuffer;
    std::string host;
    unsigned short remotePort;
    ClientNetworkModule* networkModule = nullptr;

public:
    ClientNetworkThread(NetworkHandler& handler, Buffer<Packet>& outputBuffer, const std::string& host, unsigned short port);
    ~ClientNetworkThread();

    ClientNetworkModule *getNetworkModule() const;

    void setNetworkModule(ClientNetworkModule *networkModule);

private:
    void threadCode();
    void send(sf::UdpSocket& socket, std::unique_ptr<Packet> packet, const sf::IpAddress& address, unsigned short port);
    bool processWaitingPackets(sf::UdpSocket &socket);
};