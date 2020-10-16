#pragma once

#include <vector>
#include <SFML/Network.hpp>
#include "Common/Network.h"
#include "Server/NetworkSettings.h"

/// Client connected to the server
struct UdpClient {
    const sf::IpAddress address;
    const unsigned short port;
    NetworkSettings settings;
};

class ServerNetworkHandling {
private:
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
};

