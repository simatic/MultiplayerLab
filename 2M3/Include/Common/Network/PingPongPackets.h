#pragma once

#include "Network.h"

/**
 * Ping packet, will be answered with a corresponding pong packet if no network problem arise.
 */
class PingPacket: public Packet {
private:
    unsigned long long timestamp; // Client timestamp for packet creation, can help calculate packet trip time

public:
    /**
     * Receiving side constructor. Deserialises the packet contents from a SFML packet
     * @param index sequence index of this packet
     * @param source SFML packet to read from
     */
    explicit PingPacket(PacketSequenceIndex index, sf::Packet& source);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     */
    explicit PingPacket(PacketSequenceIndex index);
    std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;
    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return Ping;
    }
};

/**
 * Answer to a ping packet
 */
class PongPacket: public Packet {
private:
    unsigned long long clientTimestamp;

public:
    /**
     * Receiving side constructor. Deserialises the packet contents from a SFML packet
     * @param index sequence index of this packet
     * @param source SFML packet to read from
     */
    explicit PongPacket(PacketSequenceIndex index, sf::Packet& source);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     * @param clientTimestamp timestamp sent by the client, used to calculate packet trip time
     */
    explicit PongPacket(PacketSequenceIndex pingPacketId, unsigned long long clientTimestamp);

    std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;
    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return Pong;
    }
};