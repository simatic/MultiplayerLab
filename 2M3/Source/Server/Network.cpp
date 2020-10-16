//
// Created by jglrxavpok on 15/10/2020.
//

#include <iostream>
#include <thread>
#include "Server/NetworkSettings.h"
#include "Common/Network.h"
#include "Server/ServerMain.h"
#include "Server/DelayCreation.h"

sf::UdpSocket socket;
std::mutex Delay::mutex4Packet4Delay;
std::vector<packet4Delay> Delay::packet4DelayList;

void networkThread(int port) {
    // Bind to port
    sf::Socket::Status status = socket.bind(port);
    if (status != sf::Socket::Done) {
        std::cerr << "Error: Problem during binding to local port " << port << " (status = " << status << ") : Another process is probably already using this port" << std::endl;
        exit(EXIT_FAILURE);
    }
    sf::UdpSocket* socketPtr = &socket;
    std::thread delayCreation(delayThread, socketPtr);

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
            packet4Delay packetToHandle;
            packetToHandle.logicalPacket = std::move(logicalPacket);
            packetToHandle.remoteAddress = remoteAddress;
            packetToHandle.remotePort = remotePort;
            Delay::mutex4Packet4Delay.lock();
            Delay::packet4DelayList.push_back(std::move(packetToHandle));
            Delay::mutex4Packet4Delay.unlock();
        }
    }
}

void killNetworkThread() {
    socket.unbind();
}

