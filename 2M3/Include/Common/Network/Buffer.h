#pragma once

#include "Network.h"

#include <memory>
#include <queue>
#include <mutex>

template <class PacketType>
class Buffer {
public:
	Buffer() = default;
	Buffer(Buffer<PacketType>& buffer);

	void addPacket(std::unique_ptr<PacketType> packet);
	void pop();

	bool empty() const;

	PacketType* getPacket();
	std::unique_ptr<PacketType> fetchPacket();

    template<class OutputPacketType>
	Buffer<OutputPacketType> extractPacketsOfType();

private:
    // mutable because we want to allow const methods to modify the mutex
    mutable std::mutex packetAccess{};
	std::deque<std::unique_ptr<PacketType>> packets;
};

#include "Buffer.inl"