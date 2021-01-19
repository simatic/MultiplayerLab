#pragma once

#include <cctype>
#include "Network.h"
#include "AddEntityPacket.h"

class WorldStatePacket: public Packet {
public:
    struct EntityInformation {
        Prefab::Type type;
        std::uint32_t id;
        bool hasTransform;
        float x;
        float y;
        float angle;

        friend sf::Packet& operator<<(sf::Packet& out, const EntityInformation& info);
        friend sf::Packet& operator>>(sf::Packet& in, EntityInformation& info);
    };

private:
    std::vector<EntityInformation> entities{};

public:
    explicit WorldStatePacket(PacketSequenceIndex index, sf::Packet& source);

    explicit WorldStatePacket(PacketSequenceIndex index, std::vector<EntityInformation> information);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return WorldState;
    }

    const std::vector<WorldStatePacket::EntityInformation>& getEntityInformation() const;

};