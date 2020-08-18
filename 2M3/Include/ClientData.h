#pragma once
#include <SFML/Network.hpp>

class ClientData
{
public:


	sf::IpAddress getAddress();
	unsigned short getPort();

private:
	sf::IpAddress mAddress;
	unsigned short mPort;

};