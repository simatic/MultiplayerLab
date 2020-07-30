#include "Client.h"

Client::Client(sf::IpAddress address, unsigned short port)
	: mAddress(address)
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

