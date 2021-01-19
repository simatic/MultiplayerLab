#pragma once

#include "Network.h"

/// Classe d'exemple pour la définition d'un packet
class TemplatePacket: public Packet {
private:
    std::string someArgument;

public:
    explicit TemplatePacket(PacketSequenceIndex index, sf::Packet& source);

    explicit TemplatePacket(PacketSequenceIndex index, std::string someArgument);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        // TODO: créer une nouvelle valeur dans l'enum PacketID pour le packet
        return PacketID::InvalidPacketID;
    }

    std::string getSomeArgument() const;

};