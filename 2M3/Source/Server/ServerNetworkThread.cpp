#include <Server/ServerClock.h>
#include "Server/ServerNetworkThread.h"
#include "Server/DelayCreation.h"

ServerNetworkThread::ServerNetworkThread(ServerNetworkHandler& handler): serverNetwork(handler), socket(handler.getSocket()) {
    backingThread = std::thread([&](){this->threadCode();});
}

ServerNetworkThread::~ServerNetworkThread() {
    backingThread.join();
}

void ServerNetworkThread::threadCode() {
    // Bind to port
    sf::Socket::Status status = socket.bind(serverNetwork.getPort());
    if (status != sf::Socket::Done) {
        std::cerr << "Error: Problem during binding to local port " << serverNetwork.getPort() << " (status = " << status << ") : Another process is probably already using this port" << std::endl;
        return;
    }

    sf::UdpSocket* socketPtr = &socket;
    auto& delayCreator = serverNetwork.getDelayCreator();
    std::thread disconnectNonConnected([&]() {serverNetwork.updateNonConnectedClients();});

    // Wait for messages on this socket
    std::cerr << "Waiting for messages on port " << serverNetwork.getPort() << std::endl;

    ready = true;
    while (delayCreator.isRunning()) {
        sf::Packet packet;
        sf::IpAddress remoteAddress;
        unsigned short remotePort;
        status = socket.receive(packet, remoteAddress, remotePort);
        if (status != sf::Socket::Done) {
            std::cerr << "Error during receive (status = " << status << ")" << std::endl;
            break;
        }

        UdpClient& client = serverNetwork.getOrCreateClient(remoteAddress, remotePort);
        serverNetwork.updateLastPacketTime(client);

        auto logicalPacket = deserializePacket(packet);
        if(logicalPacket) {
            serverNetwork.triggerEvent(client, NetworkEvent::Event{ServerClock::getInstance().get(), NetworkEvent::Type::PacketReceived,
                                                                   logicalPacket->getSequenceIndex()});
            delayCreator.delayReceivedPacket(client, std::move(logicalPacket));
        }
    }
    ready = false;

    disconnectNonConnected.join();
}

bool ServerNetworkThread::isReady() const {
    return ready;
}