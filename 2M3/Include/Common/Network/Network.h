#pragma once

#include <SFML/Network.hpp>
#include <iostream>

using ClientID = uint32_t;

class INetworkModule;

enum PacketID: sf::Uint32 {
    Ping,
    Pong,
    Echo,
    AskAddEntity,
    AddEntity,
    SetTransform,
    WorldState,
    SetColor,
    SetSpeed,
    Input,
    // TODO

    InvalidPacketID,
};

/// > 0 for server-generated indices
/// < 0 for client-generated
/// Used to keep track of sequences (Client sends A->Upon reception, the Server sends B as an answer with the same PacketSequenceIndex)
typedef sf::Int64 PacketSequenceIndex;

/**
 * 2M3 Packet layout when stored inside a SFML Packet:
 *
 * |---------------|-----------------------|-----------------------------------|
 * | Packet ID     | Packet Sequence Index | Payload                           |
 * |---------------|-----------------------|-----------------------------------|
 * | sf::Uint32    | sf::Int64             | Anything written by Packet#write  |
 * |---------------|-----------------------|-----------------------------------|
 */
class Packet {
private:
    /// used to represent a packet inside the server interface
    /// > 0 for server-generated indices
    /// < 0 for client-generated
    /// Used to keep track of sequences (Client sends A->Upon reception, the Server sends B as an answer with the same PacketSequenceIndex)
    PacketSequenceIndex sequenceIndex;

    /// ID of the client who sent this packet. Will be UINT32_MAX if the sender is the server.
    ClientID sender = std::numeric_limits<ClientID>::max();

protected:
    explicit Packet(PacketSequenceIndex index);

public:
    /// used to represent a packet inside the server interface
    PacketSequenceIndex getSequenceIndex() const;

    /// ID of the client who sent this packet. Will be UINT32_MAX if the sender is the server.
    ClientID getSender() const;

    void setSender(ClientID id);

    /// What is this packet supposed to do?
    [[nodiscard]] virtual std::unique_ptr<Packet> handle(INetworkModule* iNetworkModule) const = 0;
    /// Write this packet to SFML packet
    virtual void write(sf::Packet& destination) const = 0;

    virtual PacketID getID() const = 0;

    virtual ~Packet() = default;

    /**
     * MAKE SURE YOU DON'T WANT TO USE UdpClient#send BEFORE USING THIS METHOD.
     *
     *
     * Send a packet to the given address+port through the given socket.
     * Called 'realSend' because this method directly writes on the network, and bypasses latency and packet loss simulations.
     * @param socket
     * @param address
     * @param port
     */
    void realSend(sf::UdpSocket& socket, const sf::IpAddress& address, unsigned short port) const;
};

/**
 * Reads a 2M3 packet from a SFML packet. Handles encoding and deserializes to the correct type
 * @param packet the SFML packet to read a 2M3 packet from
 * @return the deserialized packet, or nullptr if the packet type is unknown
 */
std::unique_ptr<Packet> deserializePacket(sf::Packet& packet);
