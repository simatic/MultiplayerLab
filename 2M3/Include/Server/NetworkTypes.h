#pragma once
#include <SFML/Network.hpp>
#include "Common/Network/Network.h"
#include "Server/NetworkSettings.h"

class ServerNetworkHandler;

/// Client connected to the server
struct UdpClient {
    ServerNetworkHandler& server;
    const ClientID id;
    const sf::IpAddress address;
    const unsigned short port;
    NetworkSettings settings;

    UdpClient(ServerNetworkHandler& server, ClientID id, sf::IpAddress address, unsigned short port, NetworkSettings settings);
    void send(std::unique_ptr<Packet>&& packet) const;
};

namespace NetworkEvent {
    enum Type {
        Connected,
        Disconnected,
        PacketReceived,
        PacketDelayed,
        // When the packet is supposed to be sent
        SendingPacket,
        // When the packet is sent (with possible additional delay)
        SentPacket,

        Last
    };

    struct Event {
        const sf::Time timestamp;
        const Type type;
        const PacketSequenceIndex id;
    };

    const char* name(Type t);
}