#include <chrono>
#include "Common/EchoPacket.h"

EchoPacket::EchoPacket(int numberToWrite): Packet(Packet::newPacketIndex()) {
    number = numberToWrite;
}

EchoPacket::EchoPacket(sf::Uint64 index, sf::Packet &source): Packet(index), number(0){
    source >> number;
}

std::unique_ptr<Packet> EchoPacket::handle() const {
    std::cout << number << std::endl;
    return nullptr;
}

void EchoPacket::write(sf::Packet &destination) const {
    destination << number;
}
