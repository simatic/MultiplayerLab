#pragma once

#include "Network.h"

class SetVelocityPacket: public Packet {
private:
    std::uint32_t entityID = 0;
    float xSpeed = 0;
    float ySpeed = 0;


public:
    explicit SetVelocityPacket(PacketSequenceIndex index, sf::Packet& source);
    explicit SetVelocityPacket(PacketSequenceIndex index, std::uint32_t entityID, const sf::Vector2f& velocity);
    explicit SetVelocityPacket(PacketSequenceIndex index, std::uint32_t entityID, float xSpeed, float ySpeed);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return SetSpeed;
    }

    std::uint32_t getEntityID() const;

    float getSpeedX() const;

    float getSpeedY() const;
};