#pragma once

#include <iostream>
#include <SFML/Network.hpp>
#include "Common/Network.h"

[[noreturn]] void networkThread(int port);
void interfaceThread(const std::string& ip, const unsigned short& localPort);

[[noreturn]] void delayThread(sf::UdpSocket* socketPtr);