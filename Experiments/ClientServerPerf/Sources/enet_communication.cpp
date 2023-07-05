#include <iostream>
#include "enet_communication.h"
#include "common.h"

void broadcastPacket(ENetHost* enetHost, enet_uint8 channelID, enet_uint32 flags, std::string_view sv){
    ENetPacket* packet = enet_packet_create(sv.data(), sv.size(), flags);
    enet_host_broadcast(enetHost, channelID, packet);
    // Notice that enet_peer_send() only queues packet as to be sent and, as documentation explains,
    // Queued packets will be sent on a call to enet_host_service(). Alternatively, enet_host_flush()
    // will send out queued packets without dispatching any events.
    // ==> We do enet_host_flush() to be sure that packets is sent immediately.
    enet_host_flush(enetHost);
}

void sendPacket(ENetPeer *dest, enet_uint8 channelID, enet_uint32 flags, std::string_view sv)
{
    ENetPacket* packet = enet_packet_create(sv.data(), sv.size(), flags);
    enet_peer_send(dest, channelID, packet);
}
