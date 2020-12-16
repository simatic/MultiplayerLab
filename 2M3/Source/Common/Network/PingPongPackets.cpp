//
// Created by jglrxavpok on 15/10/2020.
//

#include <Common/Network/PingPongPackets.h>
#include <Common/Network/Network.h>
#include <chrono>

using namespace std::chrono;

PingPacket::PingPacket(PacketSequenceIndex index): Packet(index), timestamp(duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
).count()) {}

PingPacket::PingPacket(PacketSequenceIndex index, sf::Packet &source): Packet(index), timestamp(0) {
    source >> timestamp;
}

std::unique_ptr<Packet> PingPacket::handle() const {
    // send pong packet with client timestamp to measure ping on its side
    return std::make_unique<PongPacket>(getSequenceIndex(), timestamp);
}

void PingPacket::write(sf::Packet &destination) const {
    destination << timestamp;
}

PongPacket::PongPacket(PacketSequenceIndex pingPacketId, unsigned long long clientTimestamp): Packet(pingPacketId), clientTimestamp(clientTimestamp) {}

PongPacket::PongPacket(PacketSequenceIndex index, sf::Packet &source): Packet(index), clientTimestamp(0) {
    source >> clientTimestamp;
}

std::unique_ptr<Packet> PongPacket::handle() const {
    unsigned long long pingTime = duration_cast< milliseconds >(
            (system_clock::now() - milliseconds(clientTimestamp)).time_since_epoch()
    ).count();
    std::cout << "Ping took " << pingTime << " ms." << std::endl;
    return nullptr;
}

void PongPacket::write(sf::Packet &destination) const {
    destination << clientTimestamp;
}
