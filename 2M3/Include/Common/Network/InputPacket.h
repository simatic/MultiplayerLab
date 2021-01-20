#pragma once

#include "Network.h"

class InputPacket: public Packet {
private:
    bool up = false;
    bool down = false;
    bool right = false;
    bool left = false;
    bool fire = false;


public:
    explicit InputPacket(PacketSequenceIndex index, sf::Packet& source);

    explicit InputPacket(PacketSequenceIndex index, bool up, bool down, bool left, bool right, bool fire);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return Input;
    }

    bool getUp() const;

    bool getDown() const;

    bool getRight() const;

    bool getLeft() const;

    bool getFire() const;
};