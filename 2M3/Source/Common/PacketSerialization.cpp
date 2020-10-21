#include <Common/Network.h>
#include <iostream>
#include <Common/PingPongPackets.h>
#include "Common/EchoPacket.h"

std::unique_ptr<Packet> deserializePacket(sf::Packet& packet) {
    sf::Uint32 id;
    sf::Uint64 index;
    packet >> id;
    packet >> index;
    switch (id) {
        case PacketID::Ping:
            return std::make_unique<PingPacket>(index, packet);

        case PacketID::Pong:
            return std::make_unique<PongPacket>(index, packet);

        case PacketID::Echo:
            return std::make_unique<EchoPacket>(index, packet);

        default:
            std::cerr << "Unknown/unsupported packet ID " << id << std::endl;
            exit(EXIT_FAILURE);
    }
}

void Packet::send(sf::UdpSocket &socket, const sf::IpAddress &address, unsigned short port) const {
    sf::Packet packet;
    packet << getID();
    packet << getIndex();
    write(packet);
    socket.send(packet, address, port);
}

Packet::Packet(sf::Uint64 index): index(index) {}

sf::Uint64 Packet::getIndex() const {
    return index;
}

sf::Uint64 Packet::packetIndexCounter = 0;

sf::Uint64 Packet::newPacketIndex() {
    return packetIndexCounter++;
}
