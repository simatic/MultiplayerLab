#pragma once

#include "Common/Network/Network.h"

/**
 * Class to help create packets without having to worry about sequence indices.
 */
class NetworkHandler {
private:
    sf::UdpSocket socket;

protected:
    virtual PacketSequenceIndex generateNextIndex() = 0;

public:
    /**
     * Creates a new packet with the given arguments. The packet sequence index will be auto-computed by `generateNextIndex()`.
     * The arguments will be the same one as sending-side constructors of packets, without the packet sequence index.
     * Otherwise, this works like std::make_unique or std::make_shared.
     * @tparam Packet Type of packet to create
     * @tparam Args
     * @param args arguments to construct the packet
     * @return the newly created packet.
     */
    template<typename Packet, typename... Args>
    std::unique_ptr<Packet> create(Args... args);

    /**
     * Underlying socket for communications.
     * For the server, it is the socket which can receive from multiple clients and send to said clients.
     * For clients, it is the socket through all exchanges with the server happen.
     * @return
     */
    sf::UdpSocket& getSocket();

    virtual ~NetworkHandler() = default;
};

#include "NetworkHandler.inl"