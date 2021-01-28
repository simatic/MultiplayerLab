#include "Common/Network/InputPacket.h"

InputPacket::InputPacket(PacketSequenceIndex index, sf::Packet &source): Packet(index) {
    source >> up;
    source >> down;
    source >> right;
    source >> left;
    source >> fire;
}

InputPacket::InputPacket(PacketSequenceIndex index, PlayableCarInput* inputs) : Packet(index) {
    InputPacket::up = inputs->inputs->up;
    InputPacket::down = inputs->inputs->down;
    InputPacket::left = inputs->inputs->left;
    InputPacket::right = inputs->inputs->right;
    InputPacket::fire = inputs->inputs->action;
}

InputPacket::InputPacket(PacketSequenceIndex index, VehicleInput* inputs) : Packet(index) {
    InputPacket::up = inputs->up;
    InputPacket::down = inputs->down;
    InputPacket::left = inputs->left;
    InputPacket::right = inputs->right;
    InputPacket::fire = inputs->action;
}

InputPacket::InputPacket(PacketSequenceIndex index, bool up, bool down, bool left, bool right, bool fire): Packet(index) {
    InputPacket::up = up;
    InputPacket::down = down;
    InputPacket::left = left;
    InputPacket::right = right;
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
