#pragma once

#include "Network.h"

class EchoPacket: public Packet {

private:
    int number;

public:
    explicit EchoPacket(PacketSequenceIndex index, sf::Packet& source);
    explicit EchoPacket(PacketSequenceIndex index, int number);
    std::unique_ptr<Packet> handle() const override;
    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return Echo;
    }

};

