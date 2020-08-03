#include "Client.h"

Client::Client(std::string name, sf::IpAddress address, unsigned short port)
	: mName(name)
	, mAddress(address)
	, mPort(port)
{
}

sf::IpAddress Client::getAddress()
{
	return mAddress;
}

unsigned short Client::getPort()
{
	return mPort;
}

std::string Client::getName()
{
	return mName;
}

