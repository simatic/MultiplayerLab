///
/// Packets to add entities on the client or the server
///

#pragma once
#include "Network.h"
#include "Common/Prefabs/Prefabs.h"
#include <Server/Modules/ServerNetworkModule.h>

/**
 * Packet used to ask for the creation of an entity
 */
class AskAddEntityPacket: public Packet {
private:
    Prefab::Type  prefabType;

public:
    /**
     * Receiving side constructor. Deserialises the packet contents from a SFML packet
     * @param index sequence index of this packet
     * @param source SFML packet to read from
     */
    explicit AskAddEntityPacket(PacketSequenceIndex index, sf::Packet& source);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     * @param type entity type to create
     */
    explicit AskAddEntityPacket(PacketSequenceIndex index, Prefab::Type type);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return AskAddEntity;
    }
};


/**
 * Notify clients that an entity has been created.
 */
class AddEntityPacket: public Packet {
private:
    std::uint32_t networkID = 0;
    Prefab::Type  prefabType;

public:
    /**
     * Receiving side constructor. Deserialises the packet contents from a SFML packet
     * @param index sequence index of this packet
     * @param source SFML packet to read from
     */
    explicit AddEntityPacket(PacketSequenceIndex index, sf::Packet& source);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     * @param type entity type that has been created
     * @param ID entity ID to assign to the entity that has been created
     */
    explicit AddEntityPacket(PacketSequenceIndex AskAddEntityPacketId, Prefab::Type type, sf::Uint32 ID);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    Prefab::Type getEntityType() const;
    std::uint32_t getEntityID() const;

    PacketID getID() const override {
        return AddEntity;
    }
};