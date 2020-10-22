//
// Created by jglrxavpok on 15/10/2020.
//

#include <iostream>
#include <thread>
#include "Server/NetworkSettings.h"
#include "Common/Network.h"
#include "Server/ServerNetworkHandling.h"
#include "Server/ServerMain.h"
#include "Server/DelayCreation.h"

sf::UdpSocket socket;
std::mutex Delay::mutex4Packet4Delay;
std::vector<std::unique_ptr<packet4Delay>> Delay::packet4DelayList;
ClientID ServerNetworkHandling::currentClientID = 0;
std::vector<std::unique_ptr<UdpClient>> ServerNetworkHandling::clients{};
std::map<ClientID, float> ServerNetworkHandling::lastPacketTimes{};
sf::Clock timeClock;
bool ServerNetworkHandling::running = true;

[[noreturn]] void networkThread(int port) {
    // Bind to port
    sf::Socket::Status status = socket.bind(port);
    if (status != sf::Socket::Done) {
        std::cerr << "Error: Problem during binding to local port " << port << " (status = " << status << ") : Another process is probably already using this port" << std::endl;
        exit(EXIT_FAILURE);
    }

    sf::UdpSocket* socketPtr = &socket;
    std::thread delayCreation(delayThread, socketPtr);
    std::thread disconnectNonConnected(ServerNetworkHandling::updateNonConnectedClients);

    // Wait for messages on this socket
    std::cerr << "Waiting for messages on port " << port << std::endl;
    while (true) {
        sf::Packet packet;
        sf::IpAddress remoteAddress;
        unsigned short remotePort;
        status = socket.receive(packet, remoteAddress, remotePort);
        UdpClient& client = ServerNetworkHandling::getOrCreateClient(remoteAddress, remotePort);
        ServerNetworkHandling::updateLastPacketTime(client);

        if (status != sf::Socket::Done) {
            std::cerr << "Error during receive (status = " << status << ")" << std::endl;
            exit(EXIT_FAILURE);
        }

        auto logicalPacket = deserializePacket(packet);
        if(logicalPacket) {
            ServerNetworkHandling::triggerEvent(client, NetworkEvent::Event{timeClock.getElapsedTime(), NetworkEvent::Type::PacketReceived, logicalPacket->getIndex()});
            Delay::mutex4Packet4Delay.lock();
            Delay::packet4DelayList.push_back(std::make_unique<packet4Delay>(std::move(logicalPacket), client));
            Delay::mutex4Packet4Delay.unlock();
        }
    }
}

void ServerNetworkHandling::killNetworkThread() {
    running = false;
    socket.unbind();
}

UdpClient& ServerNetworkHandling::getOrCreateClient(sf::IpAddress address, unsigned short port) {
    for (auto& client : clients) {
        if(client->address == address && client->port == port) {
            return *client; // found match
        }
    }
    std::cout << "New client at " << address << ":" << port << std::endl;
    clients.push_back(std::make_unique<UdpClient>(currentClientID++, address, port, NetworkSettings()));

    auto& client = *clients.at(clients.size()-1);
    triggerEvent(client, NetworkEvent::Event{timeClock.getElapsedTime(), NetworkEvent::Type::Connected});
    return client;
}

void ServerNetworkHandling::broadcast(Packet& toBroadcast) {
    for(auto& client : clients) {
        toBroadcast.send(socket, client->address, client->port);
    }
}

void ServerNetworkHandling::updateNonConnectedClients() {
    while(running) {
        std::vector<ClientID> toDisconnect{};
        for(auto& client : clients) {
            if(lastPacketTimes[client->id]+ServerNetworkHandling::DelayBeforeDeconnection < timeClock.getElapsedTime().asSeconds()) {
                toDisconnect.push_back(client->id);
            }
        }
        for(auto& id : toDisconnect) {
            clients.erase(std::find_if(clients.begin(), clients.end(), [&](const std::unique_ptr<UdpClient>& client) { return client->id == id; }));
        }

        sf::sleep(sf::seconds(1.0f));
    }
}

void ServerNetworkHandling::updateLastPacketTime(const UdpClient &client) {
    lastPacketTimes[client.id] = timeClock.getElapsedTime().asSeconds();
}

const char *NetworkEvent::name(NetworkEvent::Type t)  {
    switch (t) {
        case Connected:
            return "Connexion";
        case Disconnected:
            return "Déconnexion";
        case PacketReceived:
            return "Packet reçu";
        case PacketDelayed:
            return "Packet délayé";
        case SendingPacket:
            return "Packet prêt à l'envoi";
        case SentPacket:
            return "Packet envoyé";

        default:
            return "Unknown type";
    }
}

UdpClient::UdpClient(ClientID id, sf::IpAddress address, unsigned short port, NetworkSettings settings): id(id), address(address), port(port), settings(settings) {}
