#pragma once

#include "Network.h"

class SetTransformPacket: public Packet {
private:
    std::uint32_t entityID = 0;
    float x = 0;
    float y = 0;
    float angle = 0;


public:
    explicit SetTransformPacket(PacketSequenceIndex index, sf::Packet& source);

    explicit SetTransformPacket(PacketSequenceIndex index, std::uint32_t entityID, float x, float y, float angle);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return SetTransform;
    }

    float getX() const;

    float getY() const;

    float getAngle() const;

    std::uint32_t getEntityID() const;
};