#include "Common/Network/AddEntityPacket.h"

AskAddEntityPacket::AskAddEntityPacket(PacketSequenceIndex index, sf::Packet& source): Packet(index) {
    sf::Uint32 type;
    source >> type;
    prefabType = static_cast<Prefab::Type>(type);
}

AskAddEntityPacket::AskAddEntityPacket(PacketSequenceIndex index, Prefab::Type type): Packet(index) {
    prefabType = type;
}

std::unique_ptr<Packet> AskAddEntityPacket::handle(INetworkModule* iNetworkModule) const {
    //return std::make_unique<ConfirmAddEntityPacket>(getSequenceIndex(), );
    return nullptr;
}

void AskAddEntityPacket::write(sf::Packet &destination) const {
    destination << static_cast<sf::Uint32>(prefabType);
}

ConfirmAddEntityPacket::ConfirmAddEntityPacket(PacketSequenceIndex index, sf::Packet& source): Packet(index) {
    source >> networkID;
    sf::Uint32 type;
    source >> type;
    prefabType = static_cast<Prefab::Type>(type);
}

ConfirmAddEntityPacket::ConfirmAddEntityPacket(PacketSequenceIndex AskAddEntityPacketId, Prefab::Type type): Packet(AskAddEntityPacketId) {
    networkID = 0; //TODO
    prefabType = type;
}

[[nodiscard]] std::unique_ptr<Packet> ConfirmAddEntityPacket::handle(INetworkModule* iNetworkModule) const {
    return nullptr;
}

void ConfirmAddEntityPacket::write(sf::Packet &destination) const {

}

AddEntityPacket::AddEntityPacket(PacketSequenceIndex index, sf::Packet& source): Packet(index) {
    source >> networkID;
    sf::Uint32 type;
    source >> type;
    prefabType = static_cast<Prefab::Type>(type);
}

AddEntityPacket::AddEntityPacket(PacketSequenceIndex AskAddEntityPacketId, Prefab::Type type): Packet(AskAddEntityPacketId){
    networkID = 0; //TODO
    prefabType = type;
}

std::unique_ptr<Packet> AddEntityPacket::handle(INetworkModule* iNetworkModule) const {
    return nullptr;
}

void AddEntityPacket::write(sf::Packet &destination) const {

}
