#include "Common/Network/WorldStatePacket.h"


WorldStatePacket::WorldStatePacket(PacketSequenceIndex index, sf::Packet& source): Packet(index) {
    // reading a vector: read its length first, then for-loop to read each element

    std::uint32_t entityCount;
    source >> entityCount;
    entities.resize(entityCount);
    for (int i = 0; i < entityCount; ++i) {
        source >> entities[i];
    }
}

WorldStatePacket::WorldStatePacket(PacketSequenceIndex index, std::vector<EntityInformation> information): Packet(index), entities(std::move(information)) {}

void WorldStatePacket::write(sf::Packet &destination) const {
    // writing a vector: write its length first, then for-loop to write each element

    destination << static_cast<std::uint32_t>(entities.size());
    for(const auto& info : entities) {
        destination << info;
    }
}

std::unique_ptr<Packet> WorldStatePacket::handle(INetworkModule* iNetworkModule) const {
    return nullptr;
}

sf::Packet& operator<<(sf::Packet& out, const WorldStatePacket::EntityInformation& info) {
    out << static_cast<sf::Uint32>(info.type);
    out << info.id;
    out << info.hasTransform;
    if(info.hasTransform) {
        out << info.x;
        out << info.y;
        out << info.angle;
    }
    return out;
}

sf::Packet& operator>>(sf::Packet& in, WorldStatePacket::EntityInformation& info) {
    sf::Uint32 typeID;
    in >> typeID;
    info.type = static_cast<Prefab::Type>(typeID);
    in >> info.id;
    in >> info.hasTransform;
    if(info.hasTransform) {
        in >> info.x;
        in >> info.y;
        in >> info.angle;
    }
    return in;
}

const std::vector<WorldStatePacket::EntityInformation>& WorldStatePacket::getEntityInformation() const {
    return entities;
}
