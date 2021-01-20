#pragma once

#include "Common/Network/Network.h"
#include "ServerNetworkHandler.h"
#include <mutex>

struct PacketToAddDelayTo {
    std::unique_ptr<Packet> logicalPacket;
    UdpClient& client;

    PacketToAddDelayTo(std::unique_ptr<Packet>&& logicalPacket, UdpClient& client): logicalPacket(std::move(logicalPacket)), client(client) {}
};

struct PacketWithDelay {
    std::unique_ptr<Packet> logicalPacket;
    const UdpClient& client;

    /// in seconds
    float delay;

    PacketWithDelay(std::unique_ptr<Packet>&& logicalPacket, const UdpClient& client, float delay): logicalPacket(std::move(logicalPacket)), client(client), delay(delay) {}
};

class DelayCreator {
private:
    std::vector<std::unique_ptr<PacketToAddDelayTo>> packet4DelayList{};
    std::mutex mutex4Packet4Delay{};
    std::vector<std::unique_ptr<PacketWithDelay>> responsePacketWithDelayList{};
    std::mutex mutex4ResponsePacketWithDelay{};
    bool stopServer = false;
    std::thread backingThread{};
    Buffer& outputBuffer;

    ServerNetworkHandler& serverNetwork;

public:
    DelayCreator(ServerNetworkHandler& serverNetworkHandler, Buffer& buffer);

    void sendToClient(const UdpClient& client, std::unique_ptr<Packet>&& packet);

    ~DelayCreator() = default;

    bool isRunning();

    void delayReceivedPacket(UdpClient& client, std::unique_ptr<Packet>&& packet);

    void join();

private:
    void threadCode();
};

