#include <Common/Network/Network.h>
#include <iostream>
#include <Common/Network/PingPongPackets.h>
#include <Server/Network/DelayCreation.h>
#include <Network/AddEntityPacket.h>
#include <Network/WorldStatePacket.h>
#include <Common/Network/SetColorPacket.h>
#include <Common/Network/SetSpeedPacket.h>
#include <Common/Network/InputPacket.h>
#include "Common/Network/EchoPacket.h"
#include "Common/Network/SetTransformPacket.h"

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

        case PacketID::SetTransform:
            return std::make_unique<SetTransformPacket>(sequenceIndex, packet);

        case PacketID::AddEntity:
            return std::make_unique<AddEntityPacket>(sequenceIndex, packet);

        case PacketID::WorldState:
            return std::make_unique<WorldStatePacket>(sequenceIndex, packet);

        case PacketID::SetColor:
            return std::make_unique<SetColorPacket>(sequenceIndex, packet);

        case PacketID::SetSpeed:
            return std::make_unique<SetVelocityPacket>(sequenceIndex, packet);

        case PacketID::Input:
            return std::make_unique<InputPacket>(sequenceIndex, packet);


        // TODO: Add your own packet for proper deserialization

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

ClientID Packet::getSender() const {
    return sender;
}

void Packet::setSender(ClientID id) {
    sender = id;
}
