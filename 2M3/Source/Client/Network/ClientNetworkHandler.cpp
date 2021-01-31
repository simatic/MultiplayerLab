#include <Client/Network/ClientNetworkHandler.h>

PacketSequenceIndex ClientNetworkHandler::generateNextIndex() {
    return --sequenceIndex;
}
