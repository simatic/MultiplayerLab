#include "Common/Network/TemplatePacket.h"

TemplatePacket::TemplatePacket(PacketSequenceIndex index, sf::Packet& source): Packet(index) {
    // TODO: lecture depuis le packet réseau
    source >> someArgument;
}

TemplatePacket::TemplatePacket(PacketSequenceIndex index, std::string someArgument): Packet(index), someArgument(someArgument) {
}

std::unique_ptr<Packet> TemplatePacket::handle(INetworkModule* iNetworkModule) const {
    return nullptr;
}

void TemplatePacket::write(sf::Packet &destination) const {
    // TODO: écrire dans le packet réseau
    destination << someArgument;
}

std::string TemplatePacket::getSomeArgument() const {
    return someArgument;
}
