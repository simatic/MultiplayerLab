#pragma once

#include <iostream>
#include <SFML/Network.hpp>
#include "Common/Network/Network.h"
#include "ServerNetworkHandler.h"

[[noreturn]] void networkThread(ServerNetworkHandler& serverNetwork, int port);
void interfaceThread(ServerNetworkHandler& serverNetwork, const std::string& ip, const unsigned short& localPort);

[[noreturn]] void delayThread(sf::UdpSocket* socketPtr);