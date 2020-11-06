#pragma once

#include "Common/Network.h"
#include <mutex>

struct packet4Delay {
    std::unique_ptr<Packet> logicalPacket;
    UdpClient& client;

    packet4Delay(std::unique_ptr<Packet>&& logicalPacket, UdpClient& client): logicalPacket(std::move(logicalPacket)), client(client) {}
};

struct packetWithDelay {
    std::unique_ptr<Packet> logicalPacket;
    UdpClient& client;

    /// in seconds
    float incomingDelay;
    float outgoingDelay;

    packetWithDelay(std::unique_ptr<Packet>&& logicalPacket, UdpClient& client, float incomingDelay, float outgoingDelay): logicalPacket(std::move(logicalPacket)), client(client), incomingDelay(incomingDelay), outgoingDelay(outgoingDelay) {}
};

class Delay {
public:
    static std::vector<std::unique_ptr<packet4Delay>> packet4DelayList;

    static std::mutex mutex4Packet4Delay;

};

