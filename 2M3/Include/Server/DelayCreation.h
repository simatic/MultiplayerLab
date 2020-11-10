#pragma once

#include "Common/Network.h"
#include "ServerNetworkHandling.h"
#include <mutex>

struct packet4Delay {
    std::unique_ptr<Packet> logicalPacket;
    UdpClient& client;

    packet4Delay(std::unique_ptr<Packet>&& logicalPacket, UdpClient& client): logicalPacket(std::move(logicalPacket)), client(client) {}
};

struct packetWithDelay {
    std::unique_ptr<Packet> logicalPacket;
    const UdpClient& client;

    /// in seconds
    float delay;

    packetWithDelay(std::unique_ptr<Packet>&& logicalPacket, const UdpClient& client, float delay): logicalPacket(std::move(logicalPacket)), client(client), delay(delay) {}
};

class Delay {
public:
    static std::vector<std::unique_ptr<packet4Delay>> packet4DelayList;

    static std::mutex mutex4Packet4Delay;

    static std::vector<std::unique_ptr<packetWithDelay>> responsePacketWithDelayList;

    static std::mutex mutex4ResponsePacketWithDelay;

};

