#pragma once

#include "Network.h"

/// Example class to create a new packet, see TemplatePacket.cpp for implementation hints
class TemplatePacket: public Packet {
private:
    // Declare payload as private fields
    std::string someArgument;
    // You are free to add more than one field.
    std::string someOtherArgument;

public:
    /**
     * Receiving side constructor.
     * This is the constructor that will be called by `deserializePacket` inside PacketSerialisation.cpp once you add your code in that file.
     *
     * Deserialises the packet contents from a SFML packet
     * @param index sequence index of this packet
     * @param source SFML packet to read from
     */
    explicit TemplatePacket(PacketSequenceIndex index, sf::Packet& source);

    /**
     * Sending side constructor. This is where you can give arguments to fill the payload.
     * @param index sequence index of this packet
     * @param someArgument your first argument
     * @param someOtherArgument your other argument
     */
    explicit TemplatePacket(PacketSequenceIndex index, std::string someArgument, std::string someOtherArgument);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        // TODO: créer une nouvelle valeur dans l'enum PacketID pour le packet
        return PacketID::InvalidPacketID;
    }

    /**
     * Expose fields via getters
     * @return the payload
     */
    std::string getSomeArgument() const;

    /**
     * Expose fields via getters
     * @return the payload
     */
    std::string getSomeOtherArgument() const;

};