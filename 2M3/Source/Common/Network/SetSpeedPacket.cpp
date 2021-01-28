#include "Common/Network/SetSpeedPacket.h"

SetVelocityPacket::SetVelocityPacket(PacketSequenceIndex index, sf::Packet &source): Packet(index) {
    source >> entityID;
    source >> xSpeed;
    source >> ySpeed;
}

SetVelocityPacket::SetVelocityPacket(PacketSequenceIndex index, std::uint32_t entityID, const sf::Vector2f& velocity): Packet(index) {
    SetVelocityPacket::entityID = entityID;
    SetVelocityPacket::xSpeed = velocity.x;
    SetVelocityPacket::ySpeed = velocity.y;
}

SetVelocityPacket::SetVelocityPacket(PacketSequenceIndex index, std::uint32_t entityID, float xSpeed, float ySpeed): Packet(index) {
    SetVelocityPacket::entityID = entityID;
    SetVelocityPacket::xSpeed = xSpeed;
    SetVelocityPacket::ySpeed = ySpeed;
}

std::unique_ptr<Packet> SetVelocityPacket::handle(INetworkModule *iNetworkModule) const {
    return nullptr;
}

void SetVelocityPacket::write(sf::Packet &destination) const {
    destination << entityID;
    destination << xSpeed;
    destination << ySpeed;
}

std::uint32_t SetVelocityPacket::getEntityID() const {
    return entityID;
}

float SetVelocityPacket::getSpeedX() const {
    return xSpeed;
}

float SetVelocityPacket::getSpeedY() const {
    return ySpeed;
}
