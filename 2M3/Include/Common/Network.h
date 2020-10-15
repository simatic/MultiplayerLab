#pragma once

#include <SFML/Network.hpp>
#include <iostream>

enum PacketID: sf::Uint32 {
    Ping,
    Pong,
    Echo,
    // TODO
};

class Packet {
public:
    /// What is this packet supposed to do?
    virtual std::unique_ptr<Packet> handle() const = 0;
    /// Write this packet to SFML packet
    virtual void write(sf::Packet& destination) const = 0;

    virtual PacketID getID() const = 0;

    virtual ~Packet() = default;

    void send(sf::UdpSocket& socket, sf::IpAddress& address, unsigned short port) const;
};

std::unique_ptr<Packet> deserializePacket(sf::Packet& packet);
