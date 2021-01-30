#pragma once

#include "Network.h"

#include <memory>
#include <queue>
#include <mutex>

/**
 * Holds a buffer of packets of a given type.
 *
 * The "front packet" in this file refers to the first packet that would be returned via a call to `fetchPacket()`
 * @tparam PacketType Type of packet to hold. Use `Packet` for a generic buffer which can hold any packet type.
 */
template <class PacketType>
class Buffer {
public:
    /**
     * Create a new empty buffer
     */
	Buffer() = default;

	/**
	 * Create a new buffer and transfer the content of the buffer passed as argument into the newly created one.
	 * @param buffer the buffer to read packets from
	 */
	Buffer(Buffer<PacketType>& buffer);

	/**
	 * Adds a new packet to this buffer
	 * @param packet the packet to add. Will be moved
	 */
	void addPacket(std::unique_ptr<PacketType> packet);

	/**
	 * Remove the first packet that would be returned via `fetchPacket()`
	 */
	void pop();

	/**
	 * Is this buffer empty?
	 */
	bool empty() const;

	/**
	 * Gets a pointer to the front packet. Will not remove it from the buffer
	 * @return
	 */
	PacketType* getPacket();

	/**
	 * Extracts the front packet to return it to the caller. The packet will be removed from the buffer.
	 * Throws if there is no packet to extract.
	 * @return the front packet
	 */
	std::unique_ptr<PacketType> fetchPacket();

	/**
	 * Extracts all packets of the given type to a new buffer for later use. The packets will be removed from the buffer.
	 * Contrary to `fetchPacket()`, this method does not throw if the buffer is empty, nor if the current buffer does
	 * not hold any packet of the given type. Calling `fetchPacket()` on the returned buffer can however throw.
	 * @tparam OutputPacketType the type of packet to extract
	 * @return a new buffer with all packets of a given type extracted from this buffer
	 */
    template<class OutputPacketType>
	Buffer<OutputPacketType> extractPacketsOfType();

private:
    // mutable because we want to allow const methods to modify the mutex
    mutable std::mutex packetAccess{};
	std::deque<std::unique_ptr<PacketType>> packets; // Packets inside this buffer
};

#include "Buffer.inl"