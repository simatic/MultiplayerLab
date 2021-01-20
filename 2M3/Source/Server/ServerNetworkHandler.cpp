//
// Created by jglrxavpok on 15/10/2020.
//

#include <iostream>
#include <thread>
#include <Server/ServerClock.h>
#include "Server/NetworkSettings.h"
#include "Server/ServerNetworkHandler.h"
#include "Server/DelayCreation.h"
#include "Server/Interface.h"

ServerNetworkHandler::ServerNetworkHandler(Buffer& buffer, const std::string& ip, unsigned short port): ip(ip), port(port) {
    delayCreator = std::make_shared<DelayCreator>(*this, buffer);
}

void ServerNetworkHandler::killNetworkThread() {
    if(running) {
        delayCreator->join();
        socket.unbind();
        running = false;
    }
}

UdpClient& ServerNetworkHandler::getOrCreateClient(sf::IpAddress address, unsigned short port) {
    for (auto& client : clients) {
        if(client->address == address && client->port == port) {
            return *client; // found match
        }
    }
    std::cout << "New client at " << address << ":" << port << std::endl;
    clients.push_back(std::make_unique<UdpClient>(*this, currentClientID++, address, port, NetworkSettings()));

    auto& client = *clients.at(clients.size()-1);
    triggerEvent(client, NetworkEvent::Event{ServerClock::getInstance().get(), NetworkEvent::Type::Connected});
    return client;
}

void ServerNetworkHandler::broadcast(std::unique_ptr<Packet> toBroadcast) {
    for(auto& client : clients) {
        client->send(std::move(toBroadcast));
    }
}

void ServerNetworkHandler::updateNonConnectedClients() {
    while(running) {
        std::vector<ClientID> toDisconnect{};
        for(auto& client : clients) {
            if(lastPacketTimes[client->id] + ServerNetworkHandler::DelayBeforeDeconnection < ServerClock::getInstance().asSeconds()) {
                toDisconnect.push_back(client->id);
            }
        }
        for(auto& id : toDisconnect) {
            clients.erase(std::find_if(clients.begin(), clients.end(), [&](const std::shared_ptr<UdpClient>& client) { return client->id == id; }));
        }

        sf::sleep(sf::seconds(1.0f));
    }
}

void ServerNetworkHandler::updateLastPacketTime(const UdpClient &client) {
    lastPacketTimes[client.id] = ServerClock::getInstance().asSeconds();
}

PacketSequenceIndex ServerNetworkHandler::generateNextIndex() {
    return ++sequenceIndex;
}

void ServerNetworkHandler::triggerEvent(const UdpClient& client, NetworkEvent::Event event) {
    for(auto& l : listeners) {
        l->onEvent(client, event);
    }
}

void ServerNetworkHandler::registerListener(IServerEventsListener *listener) {
    this->listeners.push_back(listener);
}

sf::UdpSocket& ServerNetworkHandler::getSocket() {
    return socket;
}

const std::string& ServerNetworkHandler::getIP() const {
    return ip;
}

const unsigned short ServerNetworkHandler::getPort() const {
    return port;
}

void ServerNetworkHandler::sendToClient(const UdpClient& client, std::unique_ptr<Packet>&& packet) {
    delayCreator->sendToClient(client, std::move(packet));
}

DelayCreator& ServerNetworkHandler::getDelayCreator() {
    return *delayCreator;
}

bool ServerNetworkHandler::isRunning() {
    return running;
}

const char *NetworkEvent::name(NetworkEvent::Type t)  {
    switch (t) {
        case Connected:
            return "Connexion";
        case Disconnected:
            return "Déconnexion";
        case PacketReceived:
            return "Packet envoyé par le client";
        case PacketDelayed:
            return "Packet reçu par le serveur";
        case SendingPacket:
            return "Packet envoyé par le serveur";
        case SentPacket:
            return "Packet reçu par le client";

        default:
            return "Unknown type";
    }
}

UdpClient::UdpClient(ServerNetworkHandler& server, ClientID id, sf::IpAddress address, unsigned short port, NetworkSettings settings): server(server), id(id), address(address), port(port), settings(settings) {}

void UdpClient::send(std::unique_ptr<Packet>&& packet) const {
    server.sendToClient(*this, std::move(packet));
}

INetworkModule* ServerNetworkHandler::getNetworkModule() const {
    return networkModule;
}

void ServerNetworkHandler::setNetworkModule(INetworkModule* networkModule) {
    ServerNetworkHandler::networkModule = networkModule;
}
