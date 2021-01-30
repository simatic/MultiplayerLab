#pragma once

#include <cctype>
#include "Network.h"
#include "AddEntityPacket.h"

/**
 * Packet sent by the server when a client connects to tell them about what entities are already present.
 */
class WorldStatePacket: public Packet {
public:
    /**
     * Holds basic information to send many entities at once to a client when it connects.
     */
    struct EntityInformation {
        Prefab::Type type;
        std::uint32_t id;
        bool hasTransform;
        float x;
        float y;
        float angle;

        /**
         * Write an EntityInformation struct to a SFML packet
         * @param out
         * @param info
         * @return
         */
        friend sf::Packet& operator<<(sf::Packet& out, const EntityInformation& info);

        /**
         * Read an EntityInformation struct from a SFML packet
         * @param in
         * @param info
         * @return
         */
        friend sf::Packet& operator>>(sf::Packet& in, EntityInformation& info);
    };

private:
    std::vector<EntityInformation> entities{};

public:
    /**
     * Receiving side constructor. Deserialises the packet contents from a SFML packet
     * @param index sequence index of this packet
     * @param source SFML packet to read from
     */
    explicit WorldStatePacket(PacketSequenceIndex index, sf::Packet& source);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     * @param information world state information
     */
    explicit WorldStatePacket(PacketSequenceIndex index, std::vector<EntityInformation> information);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return WorldState;
    }

    const std::vector<WorldStatePacket::EntityInformation>& getEntityInformation() const;

};