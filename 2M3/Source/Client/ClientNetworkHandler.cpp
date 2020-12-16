#include <Client/ClientNetworkHandler.h>

PacketSequenceIndex ClientNetworkHandler::generateNextIndex() {
    return --sequenceIndex;
}
