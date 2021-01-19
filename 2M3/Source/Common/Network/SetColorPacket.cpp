#include "Common/Network/SetColorPacket.h"

SetColorPacket::SetColorPacket(PacketSequenceIndex index, sf::Packet& source): Packet(index) {
    source >> entityID;
    sf::Uint8 r,g,b;
    source >> r;
    source >> g;
    source >> b;
    color = sf::Color(r,g,b);
}

SetColorPacket::SetColorPacket(PacketSequenceIndex index, std::uint32_t entityID, sf::Color color): Packet(index) {
    SetColorPacket::entityID = entityID;
    SetColorPacket::color = color;
}

[[nodiscard]] std::unique_ptr<Packet> SetColorPacket::handle(INetworkModule* iNetworkModule) const {
    return nullptr;
}

void SetColorPacket::write(sf::Packet &destination) const {
    destination << entityID;
    destination << color.r;
    destination << color.g;
    destination << color.b;
}

sf::Color SetColorPacket::getColor() const {
    return color;
}

std::uint32_t SetColorPacket::getEntityID() const {
    return entityID;
}