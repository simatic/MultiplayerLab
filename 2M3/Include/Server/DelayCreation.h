#pragma once

#include "Common/Network.h"
#include <mutex>

struct packet4Delay{
    std::unique_ptr<Packet> logicalPacket;
    sf::IpAddress remoteAddress;
    unsigned short remotePort;
};

struct packetWithDelay{
    std::unique_ptr<Packet> logicalPacket;
    sf::IpAddress remoteAddress;
    unsigned short remotePort;
    int delayMilliseconds;
};

class Delay {
public:
    static std::vector<packet4Delay> packet4DelayList;

    static std::mutex mutex4Packet4Delay;

};

