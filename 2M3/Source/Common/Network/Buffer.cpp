#include <Common/Network/Buffer.h>

void Buffer::addPacket(std::unique_ptr<Packet> packet) {
    std::lock_guard lk(packetAccess);
	packets.push_back(std::move(packet));
}

void Buffer::pop() {
    std::lock_guard lk(packetAccess);
	packets.pop_front();
}

bool Buffer::empty() const {
    std::lock_guard lk(packetAccess);
	return packets.empty();
}

Packet* Buffer::getPacket() {
    std::lock_guard lk(packetAccess);
	return packets.front().get();
}

std::unique_ptr<Packet> Buffer::fetchPacket() {
    std::lock_guard lk(packetAccess);
	std::unique_ptr<Packet> packet = std::move(packets.front());
	packets.pop_front();
	return std::move(packet);
}