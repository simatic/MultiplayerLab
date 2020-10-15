#pragma once

#include "Network.h"

class EchoPacket: public Packet {

private:
    int number;

public:
    explicit EchoPacket(sf::Packet& source);
    explicit EchoPacket(int number);
    std::unique_ptr<Packet> handle() const override;
    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return Echo;
    }

};

