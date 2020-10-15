//
// Created by jglrxavpok on 15/10/2020.
//

#include <iostream>
#include <Common/Network.h>

void networkThread(int port) {
// Bind to port
    sf::UdpSocket socket;
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

        if (status != sf::Socket::Done) {
            std::cerr << "Error during receive (status = " << status << ")" << std::endl;
            exit(EXIT_FAILURE);
        }

        auto logicalPacket = deserializePacket(packet);
        if(logicalPacket) {
            std::cout << "[Debug] Received packet with ID " << logicalPacket->getID() << std::endl;
            auto response = logicalPacket->handle();
            if(response) {
                response->send(socket, remoteAddress, remotePort);
            }
        }
    }
}