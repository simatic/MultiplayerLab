#include <Common/Network/Network.h>
#include <iostream>
#include <Common/Network/PingPongPackets.h>
#include <Server/DelayCreation.h>
#include "Common/Network/EchoPacket.h"

std::unique_ptr<Packet> deserializePacket(sf::Packet& packet) {
    sf::Uint32 id;
    PacketSequenceIndex sequenceIndex;
    packet >> id;
    packet >> sequenceIndex;
    switch (id) {
        case PacketID::Ping:
            return std::make_unique<PingPacket>(sequenceIndex, packet);

        case PacketID::Pong:
            return std::make_unique<PongPacket>(sequenceIndex, packet);

        case PacketID::Echo:
            return std::make_unique<EchoPacket>(sequenceIndex, packet);

        default:
            std::cerr << "Unknown/unsupported packet ID " << id << std::endl;
            return nullptr;
    }
}

void Packet::realSend(sf::UdpSocket &socket, const sf::IpAddress &address, unsigned short port) const {
    sf::Packet packet;
    packet << getID();
    packet << getSequenceIndex();
    write(packet);
    socket.send(packet, address, port);
}

Packet::Packet(PacketSequenceIndex index): sequenceIndex(index) {}

PacketSequenceIndex Packet::getSequenceIndex() const {
    return sequenceIndex;
}
