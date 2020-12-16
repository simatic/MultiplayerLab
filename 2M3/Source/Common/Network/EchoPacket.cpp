#include <chrono>
#include "Common/Network/EchoPacket.h"

EchoPacket::EchoPacket(PacketSequenceIndex index, int numberToWrite): Packet(index) {
    number = numberToWrite;
}

EchoPacket::EchoPacket(PacketSequenceIndex index, sf::Packet &source): Packet(index), number(0){
    source >> number;
}

std::unique_ptr<Packet> EchoPacket::handle() const {
    std::cout << number << std::endl;
    return nullptr;
}

void EchoPacket::write(sf::Packet &destination) const {
    destination << number;
}
