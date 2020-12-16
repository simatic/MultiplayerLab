#pragma once

#include "Network.h"

class PingPacket: public Packet {
private:
    unsigned long long timestamp;

public:
    explicit PingPacket(PacketSequenceIndex index, sf::Packet& source);
    explicit PingPacket(PacketSequenceIndex index);
    std::unique_ptr<Packet> handle() const override;
    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return Ping;
    }
};

class PongPacket: public Packet {
private:
    unsigned long long clientTimestamp;

public:
    explicit PongPacket(PacketSequenceIndex index, sf::Packet& source);
    explicit PongPacket(PacketSequenceIndex pingPacketId, unsigned long long clientTimestamp);

    std::unique_ptr<Packet> handle() const override;
    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return Pong;
    }
};