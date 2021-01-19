#pragma once

#include "Network.h"
#include <SFML/Graphics/Color.hpp>

class SetColorPacket: public Packet {
private:
    std::uint32_t entityID = 0;
    sf::Color color = sf::Color::Blue;


public:
    explicit SetColorPacket(PacketSequenceIndex index, sf::Packet& source);

    explicit SetColorPacket(PacketSequenceIndex index, std::uint32_t entityID, sf::Color color);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return SetColor;
    }

    sf::Color getColor() const;

    std::uint32_t getEntityID() const;
};
