#include <Common/Network.h>
#include <iostream>
#include <Common/PingPongPackets.h>
#include "Common/EchoPacket.h"

std::unique_ptr<Packet> deserializePacket(sf::Packet& packet) {
    sf::Uint32 id;
    packet >> id;
    switch (id) {
        case PacketID::Ping:
            return std::make_unique<PingPacket>(packet);

        case PacketID::Pong:
            return std::make_unique<PongPacket>(packet);

        case PacketID::Echo:
            return std::make_unique<EchoPacket>(packet);

        default:
            std::cerr << "Unknown/unsupported packet ID " << id << std::endl;
            exit(EXIT_FAILURE);
    }
}

void Packet::send(sf::UdpSocket &socket, sf::IpAddress &address, unsigned short port) const {
    sf::Packet packet;
    packet << getID();
    write(packet);
    socket.send(packet, address, port);
}
