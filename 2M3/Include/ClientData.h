#pragma once
#include <SFML/Network.hpp>

class ClientData
{
public:
	ClientData(sf::Uint32 id, sf::IpAddress address, unsigned short port);

	sf::Uint32 getID();
	sf::IpAddress& getAddress();
	unsigned short getPort();
	sf::Time getDelay();
	void setDelay(sf::Time t);

private:
	sf::Uint32 mID;

	sf::IpAddress mAddress;
	unsigned short mPort;

	sf::Time mDelay;

};