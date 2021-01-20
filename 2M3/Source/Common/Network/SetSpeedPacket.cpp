#include "Common/Network/SetSpeedPacket.h"

SetSpeedPacket::SetSpeedPacket(PacketSequenceIndex index, sf::Packet &source): Packet(index) {
    source >> xSpeed;
    source >> ySpeed;
}

SetSpeedPacket::SetSpeedPacket(PacketSequenceIndex index, float xSpeed, float ySpeed): Packet(index) {
    SetSpeedPacket::xSpeed = xSpeed;
    SetSpeedPacket::ySpeed = ySpeed;
}

std::unique_ptr<Packet> SetSpeedPacket::handle(INetworkModule *iNetworkModule) const {
    return nullptr;
}

void SetSpeedPacket::write(sf::Packet &destination) const {
    destination << xSpeed;
    destination << ySpeed;
}

float SetSpeedPacket::getSpeedX() const {
    return xSpeed;
}

float SetSpeedPacket::getSpeedY() const {
    return ySpeed;
}
