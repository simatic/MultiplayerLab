#pragma once

#include "Network.h"

#include <memory>
#include <queue>
#include <mutex>

class Buffer {
public:
	Buffer() = default;

	void addPacket(std::unique_ptr<Packet> packet);
	void pop();

	bool empty() const;

	Packet* getPacket();
	std::unique_ptr<Packet> fetchPacket();

    template<class Type>
	std::queue<std::unique_ptr<Type>> extractPacketsOfType();

private:
    // mutable because we want to allow const methods to modify the mutex
    mutable std::mutex packetAccess{};
	std::deque<std::unique_ptr<Packet>> packets;
};

template<class Type>
std::queue<std::unique_ptr<Type>> Buffer::extractPacketsOfType() {
    std::lock_guard lk(packetAccess);
    std::queue<std::unique_ptr<Type>> result{};
    for(auto it = packets.begin(); it != packets.end(); ) {
        auto& packetAtPosition = *it;
        // instance of Type
        if(Type* cast = dynamic_cast<Type*>(packetAtPosition.get())) {
            auto packet = std::move(*it);
            // dangerous ""dynamic_cast"" for a unique_ptr
            //  should be fine thanks to the dynamic_cast check above
            // https://stackoverflow.com/a/26377517
            std::unique_ptr<Type> converted(cast);
            packet.release();
            result.push(std::move(converted));
            it = packets.erase(it);
        } else {
            it++;
        }
    }
    return std::move(result);
}