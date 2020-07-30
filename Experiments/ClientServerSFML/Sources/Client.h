#pragma once

#include <SFML/Network.hpp>

class Client
{
public:
	Client(sf::IpAddress address, unsigned short port);
	sf::IpAddress getAddress();
	unsigned short getPort();
private:
	sf::IpAddress mAddress;
	unsigned short mPort;
};

