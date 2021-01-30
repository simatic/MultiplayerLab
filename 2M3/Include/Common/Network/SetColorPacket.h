#pragma once

#include "Network.h"
#include <SFML/Graphics/Color.hpp>

/**
 * Sets the color of a given entity. Intended to inform players of their own color and of other players' colors.
 */
class SetColorPacket: public Packet {
private:
    std::uint32_t entityID = 0;
    sf::Color color = sf::Color::Blue;


public:
    /**
     * Receiving side constructor. Deserialises the packet contents from a SFML packet
     * @param index sequence index of this packet
     * @param source SFML packet to read from
     */
    explicit SetColorPacket(PacketSequenceIndex index, sf::Packet& source);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     * @param entityID which entity to color
     * @param color the color to apply
     */
    explicit SetColorPacket(PacketSequenceIndex index, std::uint32_t entityID, sf::Color color);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return SetColor;
    }

    sf::Color getColor() const;

    std::uint32_t getEntityID() const;
};
