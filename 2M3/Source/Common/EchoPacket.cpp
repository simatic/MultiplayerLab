#include <chrono>
#include "Common/EchoPacket.h"

EchoPacket::EchoPacket(sf::Packet &source) : number(0){
    source >> number;
}

EchoPacket::EchoPacket(int numberToWrite){
    number = numberToWrite;
}

std::unique_ptr<Packet> EchoPacket::handle() const {
    std::cout << number << std::endl;
    return nullptr;
}

void EchoPacket::write(sf::Packet &destination) const {
    destination << number;
}
