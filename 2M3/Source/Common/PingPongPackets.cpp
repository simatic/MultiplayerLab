//
// Created by jglrxavpok on 15/10/2020.
//

#include <Common/PingPongPackets.h>
#include <Common/Network.h>
#include <chrono>

using namespace std::chrono;

PingPacket::PingPacket(): timestamp(duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
).count()) {}

PingPacket::PingPacket(sf::Packet &source): timestamp(0) {
    source >> timestamp;
}

std::unique_ptr<Packet> PingPacket::handle() const {
    // send pong packet with client timestamp to measure ping on its side
    return std::make_unique<PongPacket>(timestamp);
}

void PingPacket::write(sf::Packet &destination) const {
    destination << timestamp;
}

PongPacket::PongPacket(unsigned long long clientTimestamp): clientTimestamp(clientTimestamp) {}

PongPacket::PongPacket(sf::Packet &source): clientTimestamp(0) {
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
