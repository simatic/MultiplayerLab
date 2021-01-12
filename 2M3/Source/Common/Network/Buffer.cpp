#include <Common/Network/Buffer.h>

void Buffer::addPacket(std::unique_ptr<Packet> packet) {
	packets.push(std::move(packet));
}

void Buffer::pop() {
	packets.pop();
}

bool Buffer::empty() const {
	return packets.empty();
}

Packet* Buffer::getPacket() {
	return packets.front().get();
}

std::unique_ptr<Packet> Buffer::fetchPacket() {
	std::unique_ptr<Packet> packet = std::move(packets.front());
	packets.pop();
	return std::move(packet);
}