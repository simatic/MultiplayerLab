#pragma once

#include <string>
#include <SFML/Network.hpp>

class Client
{
public:
	Client(std::string name, sf::IpAddress address, unsigned short port);
	sf::IpAddress getAddress();
	unsigned short getPort();
	std::string getName();
private:
	sf::IpAddress mAddress;
	unsigned short mPort;
	std::string mName;
};

