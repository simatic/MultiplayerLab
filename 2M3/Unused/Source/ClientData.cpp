#include <ClientData.h>

ClientData::ClientData(sf::Uint32 id, sf::IpAddress address, unsigned short port) :
	mID(id),
	mAddress(address),
	mPort(port)
{
}

sf::Uint32 ClientData::getID()
{
	return mID;
}

sf::IpAddress& ClientData::getAddress()
{
	return mAddress;
}

unsigned short ClientData::getPort()
{
	return mPort;
}

sf::Time ClientData::getDelay()
{
	return mDelay;
}

void ClientData::setDelay(sf::Time t)
{
	mDelay = t;
}
