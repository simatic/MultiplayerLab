#pragma once

#include "Common/Network/Network.h"

class NetworkHandler {
protected:
    virtual PacketSequenceIndex generateNextIndex() = 0;

public:
    template<typename Packet, typename... Args>
    std::unique_ptr<Packet> create(Args... args);

    virtual ~NetworkHandler() = default;
};

#include "NetworkHandler.inl"