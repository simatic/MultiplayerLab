#include <GameClient.h>
#include <NetworkCommon.h>

sf::IpAddress GameClient::getAddress()
{
	return mAddress;
}

unsigned short GameClient::getPort()
{
	return mPort;
}

void GameClient::processWaitingPackets(sf::UdpSocket& socket)
{
}

void GameClient::processReceivedPacket(sf::UdpSocket& socket, sf::Packet& packet, sf::IpAddress& remoteAddress, unsigned short remotePort)
{
}
