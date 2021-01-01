//
// Created by jglrxavpok on 15/10/2020.
//

#include <Common/Network/Network.h>
#include <Common/Network/PingPongPackets.h>
#include <iostream>
#include <thread>
#include <Client/ClientNetworkThread.h>

void ClientNetworkThread::send(sf::UdpSocket& socket, std::unique_ptr<Packet> packet, const sf::IpAddress& address, unsigned short port){
    packet->realSend(socket, address, port);
}

bool ClientNetworkThread::processWaitingPackets(sf::UdpSocket &socket)
{
    bool failed = false;
    sf::Socket::Status status;
    do
    {
        // We try to see if there is a message to process
        sf::Packet packet;
        sf::IpAddress remoteAddress;
        unsigned short localPort;
        status = socket.receive(packet, remoteAddress, localPort);
        if (status == sf::Socket::NotReady)
            break;

        // We process the message
        auto logicalPacket = deserializePacket(packet);
        if(logicalPacket) {
            auto response = logicalPacket->handle();
            if(response) {
                send(socket, std::move(response), remoteAddress, remotePort);
            }
        } else {
            // invalid packet, stop
            failed = true;
            break;
        }
    }
    while (true); // We exit this loop thanks to break instruction when ((status == sf::Socket::NotReady) || (status == sf::Socket::Disconnected))
    return !failed;
}

void ClientNetworkThread::threadCode() {
    sf::IpAddress ip(host);
    sf::UdpSocket& socket = network.getSocket();
    sf::Socket::Status status = socket.bind(sf::Socket::AnyPort);
    unsigned short localPort = socket.getLocalPort();
    if (status != sf::Socket::Done)
    {
        std::cerr << "Error: Problem during binding to local port " << localPort << " (status = " << status << ") : Another process is probably already using this port" << std::endl;
        return;
    }
    socket.setBlocking(false);
    send(socket, network.create<PingPacket>(), ip, remotePort);

    sf::sleep(sf::milliseconds(1));
    int number = 0;
    while(true) {
        if(!processWaitingPackets(socket)) {
            return;
        }
        sf::sleep(sf::milliseconds(100));
        // TODO: replace with game code?
        //network.create<PingPacket>()->realSend(socket, ip, remotePort);
        number++;
        //EchoPacket(number).send(socket, ip, remotePort);
        sf::sleep(sf::milliseconds(1));
    }
    return;
}

ClientNetworkThread::ClientNetworkThread(NetworkHandler& handler, const std::string& host, unsigned short port): network(handler), host(host), remotePort(port) {
    backingThread = std::thread([&]() { this->threadCode();});
}

ClientNetworkThread::~ClientNetworkThread() {
    backingThread.join();
}
