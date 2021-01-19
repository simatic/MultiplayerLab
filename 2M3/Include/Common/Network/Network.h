#pragma once

#include <SFML/Network.hpp>
#include <iostream>

class INetworkModule;

enum PacketID: sf::Uint32 {
    Ping,
    Pong,
    Echo,
    AskAddEntity,
    AddEntity,
    SetTransform,
    WorldState,
    // TODO

    InvalidPacketID,
};

/// > 0 for server-generated indices
/// < 0 for client-generated
/// Used to keep track of sequences (Client sends A->Upon reception, the Server sends B as an answer with the same PacketSequenceIndex)
typedef sf::Int64 PacketSequenceIndex;

class Packet {
private:
    /// used to represent a packet inside the server interface
    /// > 0 for server-generated indices
    /// < 0 for client-generated
    /// Used to keep track of sequences (Client sends A->Upon reception, the Server sends B as an answer with the same PacketSequenceIndex)
    PacketSequenceIndex sequenceIndex;

protected:
    explicit Packet(PacketSequenceIndex index);

public:
    /// used to represent a packet inside the server interface
    PacketSequenceIndex getSequenceIndex() const;

    /// What is this packet supposed to do?
    [[nodiscard]] virtual std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const = 0;
    /// Write this packet to SFML packet
    virtual void write(sf::Packet& destination) const = 0;

    virtual PacketID getID() const = 0;

    virtual ~Packet() = default;

    void realSend(sf::UdpSocket& socket, const sf::IpAddress& address, unsigned short port) const;
};

std::unique_ptr<Packet> deserializePacket(sf::Packet& packet);
