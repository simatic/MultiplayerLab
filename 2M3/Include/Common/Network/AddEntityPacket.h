#pragma once
#include "Network.h"
#include "Common/Prefabs/Prefabs.h"
#include <Server/Modules/ServerNetworkModule.h>


//Ask for the creation of an entity
class AskAddEntityPacket: public Packet {
private:
    Prefab::Type  prefabType;

public:
    explicit AskAddEntityPacket(PacketSequenceIndex index, sf::Packet& source);

    explicit AskAddEntityPacket(PacketSequenceIndex index, Prefab::Type type);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return AskAddEntity;
    }
};


//Notify other clients that an entity has been created.
class AddEntityPacket: public Packet {
private:
    std::uint32_t networkID = 0;
    Prefab::Type  prefabType;

public:
    explicit AddEntityPacket(PacketSequenceIndex index, sf::Packet& source);

    explicit AddEntityPacket(PacketSequenceIndex AskAddEntityPacketId, Prefab::Type type, sf::Uint32 ID);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    Prefab::Type getEntityType() const;
    std::uint32_t getEntityID() const;

    PacketID getID() const override {
        return AddEntity;
    }
};