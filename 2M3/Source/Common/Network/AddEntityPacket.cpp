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
    auto networkModule = dynamic_cast<ServerNetworkModule*>(iNetworkModule);
    sf::Uint32 networkID;
    networkID = networkModule->getNewNetworkID();
    return std::make_unique<AddEntityPacket>(getSequenceIndex(), prefabType, networkID);
}

void AskAddEntityPacket::write(sf::Packet &destination) const {
    destination << static_cast<sf::Uint32>(prefabType);
}


AddEntityPacket::AddEntityPacket(PacketSequenceIndex index, sf::Packet& source): Packet(index) {
    source >> networkID;
    sf::Uint32 type;
    source >> type;
    prefabType = static_cast<Prefab::Type>(type);
}

AddEntityPacket::AddEntityPacket(PacketSequenceIndex AskAddEntityPacketId, Prefab::Type type, sf::Uint32 ID):
Packet(AskAddEntityPacketId){
    networkID = ID;
    prefabType = type;
}

std::unique_ptr<Packet> AddEntityPacket::handle(INetworkModule* iNetworkModule) const {
    return nullptr;
}

void AddEntityPacket::write(sf::Packet &destination) const {
    destination << networkID;
    destination << prefabType;
}