#pragma once

#include <iostream>
#include <SFML/Network.hpp>
#include "Common/Network.h"

void networkThread(int port);
void killNetworkThread();
void interfaceThread();
void delayThread(sf::UdpSocket* socketPtr);