#pragma once

#include "Network.h"
#include <Common/Components/Transform.h>

/**
 * Sets the position and angle of a given entity
 */
class SetTransformPacket: public Packet {
private:
    std::uint32_t entityID = 0;
    float x = 0;
    float y = 0;
    float angle = 0;


public:
    /**
     * Receiving side constructor. Deserialises the packet contents from a SFML packet
     * @param index sequence index of this packet
     * @param source SFML packet to read from
     */
    explicit SetTransformPacket(PacketSequenceIndex index, sf::Packet& source);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     * @param entityID entity to apply changes to
     * @param transform component to copy translation and rotation from
     */
    explicit SetTransformPacket(PacketSequenceIndex index, std::uint32_t entityID, Transform* transform);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     * @param entityID entity to apply changes to
     * @param x position on the X axis
     * @param y position on the Y axis
     * @param angle entity angle
     */
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