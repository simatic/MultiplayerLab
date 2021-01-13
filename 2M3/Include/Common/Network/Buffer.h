#pragma once

#include "Network.h"

#include <memory>
#include <queue>

class Buffer {
public:
	Buffer() = default;

	void addPacket(std::unique_ptr<Packet> packet);
	void pop();

	bool empty() const;

	Packet* getPacket();
	std::unique_ptr<Packet> fetchPacket();

private:
	std::queue<std::unique_ptr<Packet>> packets;
};