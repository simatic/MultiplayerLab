#pragma once

#include "Network.h"

class PingPacket: public Packet {
private:
    unsigned long long timestamp;

public:
    explicit PingPacket(sf::Uint64 index, sf::Packet& source);
    explicit PingPacket();
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
    explicit PongPacket(sf::Uint64 index, sf::Packet& source);
    explicit PongPacket(sf::Uint64 pingPacketId, unsigned long long clientTimestamp);

    std::unique_ptr<Packet> handle() const override;
    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return Pong;
    }
};