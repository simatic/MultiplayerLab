#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <Server/ServerNetworkHandling.h>

enum PacketID: sf::Uint32 {
    Ping,
    Pong,
    Echo,
    // TODO
};

class Packet {
private:
    /// used to represent a packet inside the server interface
    sf::Uint64 index;

protected:
    explicit Packet(sf::Uint64 index);

public:
    /// used to represent a packet inside the server interface
    sf::Uint64 getIndex() const;

    /// What is this packet supposed to do?
    virtual std::unique_ptr<Packet> handle() const = 0;
    /// Write this packet to SFML packet
    virtual void write(sf::Packet& destination) const = 0;

    virtual PacketID getID() const = 0;

    virtual ~Packet() = default;

    void realSend(sf::UdpSocket& socket, const sf::IpAddress& address, unsigned short port) const;

    // static members
private:
    static sf::Uint64 packetIndexCounter;

public:
    static sf::Uint64 newPacketIndex();
};

std::unique_ptr<Packet> deserializePacket(sf::Packet& packet);
