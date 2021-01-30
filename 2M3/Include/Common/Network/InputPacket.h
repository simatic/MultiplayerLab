#pragma once

#include "Network.h"
#include <Common/Components/PlayableVehicleInput.h>
#include <Common/Components/VehicleInput.h>

class InputPacket: public Packet {
private:
    bool up = false;
    bool down = false;
    bool right = false;
    bool left = false;
    bool fire = false;


public:
    /**
     * Receiving side constructor. Deserialises the packet contents from a SFML packet
     * @param index sequence index of this packet
     * @param source SFML packet to read from
     */
    explicit InputPacket(PacketSequenceIndex index, sf::Packet& source);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     * @param inputs component to extract inputs from
     */
    explicit InputPacket(PacketSequenceIndex index, PlayableVehicleInput* inputs);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     * @param inputs component to extract inputs from
     */
    explicit InputPacket(PacketSequenceIndex index, VehicleInput* inputs);

    /**
     * Sending side constructor.
     * @param index sequence index of this packet
     * @param up is up pressed?
     * @param down is down pressed?
     * @param left is left pressed?
     * @param right is right pressed?
     * @param fire is fire pressed?
     */
    explicit InputPacket(PacketSequenceIndex index, bool up, bool down, bool left, bool right, bool fire);

    [[nodiscard]] std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const override;

    void write(sf::Packet &destination) const override;

    PacketID getID() const override {
        return Input;
    }

    bool getUp() const;

    bool getDown() const;

    bool getRight() const;

    bool getLeft() const;

    bool getFire() const;
};