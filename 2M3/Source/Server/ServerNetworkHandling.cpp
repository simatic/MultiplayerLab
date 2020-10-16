//
// Created by jglrxavpok on 15/10/2020.
//

#include <iostream>
#include "Server/NetworkSettings.h"
#include "Common/Network.h"
#include "Server/ServerNetworkHandling.h"

sf::UdpSocket socket;
std::vector<UdpClient> ServerNetworkHandling::clients{};

void networkThread(int port) {
    // Bind to port
    sf::Socket::Status status = socket.bind(port);
    if (status != sf::Socket::Done) {
        std::cerr << "Error: Problem during binding to local port " << port << " (status = " << status << ") : Another process is probably already using this port" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Wait for messages on this socket
    std::cerr << "Waiting for messages on port " << port << std::endl;
    while (true) {
        sf::Packet packet;
        sf::IpAddress remoteAddress;
        unsigned short remotePort;
        status = socket.receive(packet, remoteAddress, remotePort);
        UdpClient& client = ServerNetworkHandling::getOrCreateClient(remoteAddress, remotePort);

        if (status != sf::Socket::Done) {
            std::cerr << "Error during receive (status = " << status << ")" << std::endl;
            exit(EXIT_FAILURE);
        }

        auto logicalPacket = deserializePacket(packet);
        if(logicalPacket) {
            std::cout << "[Debug] Received packet with ID " << logicalPacket->getID() << std::endl;
            if(!client.settings.inComingPacketLost()){
                auto response = logicalPacket->handle();
                if(response) {
                    if(!client.settings.outGoingPacketLost()){
                        response->send(socket, remoteAddress, remotePort);
                    }
                }
            }
        }
    }
}

void killNetworkThread() {
    socket.unbind();
}

UdpClient& ServerNetworkHandling::getOrCreateClient(sf::IpAddress address, unsigned short port) {
    for (auto& client : clients) {
        if(client.address == address && client.port == port) {
            return client; // found match
        }
    }
    std::cout << "New client at " << address << ":" << port << std::endl;
    clients.push_back({address, port, NetworkSettings()});
    return clients.at(clients.size()-1);
}

void ServerNetworkHandling::broadcast(Packet& toBroadcast) {
    for(auto& client : clients) {
        toBroadcast.send(socket, client.address, client.port);
    }
}
