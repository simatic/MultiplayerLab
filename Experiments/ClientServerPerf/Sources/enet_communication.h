#pragma once

#include <string>
#include <enet/enet.h>

constexpr enet_uint32 timeoutFor_enet_host_service{20}; // Defined in milliseconds (value determined by doing experiments with different values)

void broadcastPacket(ENetHost* enetHost, enet_uint8 channelID, enet_uint32 flags, std::string_view sv);
void sendPacket(ENetPeer *dest, enet_uint8 channelID, enet_uint32 flags, std::string_view sv);
