#pragma once

#include "Network.h"

/**
 * Packet to perform an echo to the server: send a number, the server will send it back to you.
 * Can help debugging communications.
 */
class EchoPacket: public Packet {

private:
    int number;

public:
    /**
     * Receiving side constructor. Deserialises the packet contents from a SFML packet
     * @param index sequence index of this packet
     * @param source SFML packet to read from
     */
    explicit EchoPacket(PacketSequenceIndex index, sf::Packet& source);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     * @param number the number
     */
    explicit EchoPacket(PacketSequenceIndex index, int number);
    std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;
    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return Echo;
    }

};

