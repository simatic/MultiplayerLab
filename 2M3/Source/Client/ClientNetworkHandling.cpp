#include <Common/Network.h>

PacketSequenceIndex Packet::packetIndexCounter = -1;

PacketSequenceIndex Packet::newPacketIndex() {
    return packetIndexCounter--;
}