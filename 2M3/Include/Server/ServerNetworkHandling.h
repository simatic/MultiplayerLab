#pragma once

#include <vector>
#include <SFML/Network.hpp>
#include "Common/Network.h"
#include "Server/NetworkSettings.h"

using ClientID = unsigned int;

/// Client connected to the server
struct UdpClient {
    const ClientID id;
    const sf::IpAddress address;
    const unsigned short port;
    NetworkSettings settings;
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
        const sf::Uint64 id;
    };

    const char* name(Type t);
}

class ServerNetworkHandling {
private:
    static ClientID currentClientID;
    static std::vector<UdpClient> clients;

public:
    /// Returns the client with the given address and port.
    /// Creates a new client if none match.
    static UdpClient& getOrCreateClient(sf::IpAddress address, unsigned short port);

    /// Send a packet to all clients
    static void broadcast(Packet& toBroadcast);

    static std::vector<UdpClient>& getClients() {
        return clients;
    }

    static void triggerEvent(UdpClient& client, NetworkEvent::Event event);
};

