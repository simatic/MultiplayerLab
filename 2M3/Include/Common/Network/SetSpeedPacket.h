#pragma once

#include "Network.h"

class SetSpeedPacket: public Packet {
private:
    float xSpeed = 0;
    float ySpeed = 0;


public:
    explicit SetSpeedPacket(PacketSequenceIndex index, sf::Packet& source);

    explicit SetSpeedPacket(PacketSequenceIndex index, float xSpeed, float ySpeed);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return SetSpeed;
    }

    float getSpeedX() const;

    float getSpeedY() const;
};