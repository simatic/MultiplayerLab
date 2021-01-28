#include "Buffer.h"

template <class PacketType>
Buffer<PacketType>::Buffer(Buffer<PacketType>& buffer) :
    packetAccess()
{
    while (!buffer.empty()) {
        packets.push_back(buffer.fetchPacket());
    }
}

template <class PacketType>
void Buffer<PacketType>::addPacket(std::unique_ptr<PacketType> packet) {
	std::lock_guard lk(packetAccess);
	packets.push_back(std::move(packet));
}

template <class PacketType>
void Buffer<PacketType>::pop() {
	std::lock_guard lk(packetAccess);
	packets.pop_front();
}

template <class PacketType>
bool Buffer<PacketType>::empty() const {
	std::lock_guard lk(packetAccess);
	return packets.empty();
}

template <class PacketType>
PacketType* Buffer<PacketType>::getPacket() {
	std::lock_guard lk(packetAccess);
	return packets.front().get();
}

template <class PacketType>
std::unique_ptr<PacketType> Buffer<PacketType>::fetchPacket() {
	std::lock_guard lk(packetAccess);
	std::unique_ptr<PacketType> packet = std::move(packets.front());
	packets.pop_front();
	return std::move(packet);
}

template <class PacketType>
template <class OutputPacketType>
Buffer<OutputPacketType> Buffer<PacketType>::extractPacketsOfType() {
    std::lock_guard lk(packetAccess);
    Buffer<OutputPacketType> result{};
    for (auto it = packets.begin(); it != packets.end(); ) {
        auto& packetAtPosition = *it;
        // instance of Type
        if (OutputPacketType* cast = dynamic_cast<OutputPacketType*>(packetAtPosition.get())) {
            auto packet = std::move(*it);
            // dangerous ""dynamic_cast"" for a unique_ptr
            //  should be fine thanks to the dynamic_cast check above
            // https://stackoverflow.com/a/26377517
            std::unique_ptr<OutputPacketType> converted(cast);
            packet.release();
            result.addPacket(std::move(converted));
            it = packets.erase(it);
        }
        else {
            it++;
        }
    }
    return result;
}