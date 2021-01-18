#include "Common/Network/SetTransformPacket.h"


SetTransformPacket::SetTransformPacket(PacketSequenceIndex index, sf::Packet& source): Packet(index) {
    source >> x;
    source >> y;
    source >> angle;
}

SetTransformPacket::SetTransformPacket(PacketSequenceIndex index, float x, float y, float angle): Packet(index) {
    SetTransformPacket::x = x;
    SetTransformPacket::y = y;
    SetTransformPacket::angle = angle;
}

std::unique_ptr<Packet> SetTransformPacket::handle(INetworkModule* iNetworkModule) const {
    return nullptr;
}

void SetTransformPacket::write(sf::Packet &destination) const {
    destination << x;
    destination << y;
    destination << angle;
}

float SetTransformPacket::getX() const {
    return x;
}

float SetTransformPacket::getY() const {
    return y;
}

float SetTransformPacket::getAngle() const {
    return angle;
}