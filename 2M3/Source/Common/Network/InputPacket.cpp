#include "Common/Network/InputPacket.h"

InputPacket::InputPacket(PacketSequenceIndex index, sf::Packet &source): Packet(index) {
    source >> up;
    source >> down;
    source >> right;
    source >> left;
    source >> fire;
}

InputPacket::InputPacket(PacketSequenceIndex index, bool up, bool down, bool right, bool left, bool fire): Packet(index) {
    InputPacket::up = up;
    InputPacket::down = down;
    InputPacket::right = right;
    InputPacket::left = left;
    InputPacket::fire = fire;
}

std::unique_ptr<Packet> InputPacket::handle(INetworkModule *iNetworkModule) const {
    return nullptr;
}

void InputPacket::write(sf::Packet &destination) const {
    destination << up;
    destination << down;
    destination << right;
    destination << left;
    destination << fire;
}

bool InputPacket::getUp() const {
    return up;
}

bool InputPacket::getDown() const {
    return down;
}

bool InputPacket::getRight() const {
    return right;
}

bool InputPacket::getLeft() const {
    return left;
}

bool InputPacket::getFire() const {
    return fire;
}
