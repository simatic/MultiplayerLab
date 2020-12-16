#pragma once
#include <Common/Network/NetworkHandler.h>

class ClientNetworkHandler: public NetworkHandler {
private:
    PacketSequenceIndex sequenceIndex = 0;

protected:
    PacketSequenceIndex generateNextIndex() override;
};