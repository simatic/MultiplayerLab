#pragma once

#include "Network.h"

/**
 * Sets the velocity of a given entity
 */
class SetVelocityPacket: public Packet {
private:
    std::uint32_t entityID = 0;
    float xSpeed = 0;
    float ySpeed = 0;


public:
    /**
     * Receiving side constructor. Deserialises the packet contents from a SFML packet
     * @param index sequence index of this packet
     * @param source SFML packet to read from
     */
    explicit SetVelocityPacket(PacketSequenceIndex index, sf::Packet& source);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     * @param velocity velocity to set
     */
    explicit SetVelocityPacket(PacketSequenceIndex index, std::uint32_t entityID, const sf::Vector2f& velocity);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     * @param xSpeed velocity to set on the X axis
     * @param ySpeed velocity to set on the Y axis
     */
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