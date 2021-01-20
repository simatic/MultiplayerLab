#include "Common/Network/SetSpeedPacket.h"

SetSpeedPacket::SetSpeedPacket(PacketSequenceIndex index, sf::Packet &source): Packet(index) {
    source >> entityID;
    source >> xSpeed;
    source >> ySpeed;
}

SetSpeedPacket::SetSpeedPacket(PacketSequenceIndex index, std::uint32_t entityID, float xSpeed, float ySpeed): Packet(index) {
    SetSpeedPacket::entityID = entityID;
    SetSpeedPacket::xSpeed = xSpeed;
    SetSpeedPacket::ySpeed = ySpeed;
}

std::unique_ptr<Packet> SetSpeedPacket::handle(INetworkModule *iNetworkModule) const {
    return nullptr;
}

void SetSpeedPacket::write(sf::Packet &destination) const {
    destination << entityID;
    destination << xSpeed;
    destination << ySpeed;
}

std::uint32_t SetSpeedPacket::getEntityID() const {
    return entityID;
}

float SetSpeedPacket::getSpeedX() const {
    return xSpeed;
}

float SetSpeedPacket::getSpeedY() const {
    return ySpeed;
}
