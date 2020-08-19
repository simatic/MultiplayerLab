#pragma once
#include <SFML/Network.hpp>

class GameClient
{
public:


	sf::IpAddress getAddress();
	unsigned short getPort();

	void processWaitingPackets(sf::UdpSocket& socket);
	void processReceivedPacket(sf::UdpSocket& socket, sf::Packet& packet, sf::IpAddress& remoteAddress, unsigned short remotePort);

private:
	sf::Uint32 mID;
	sf::IpAddress mAddress;
	unsigned short mPort;

	sf::IpAddress mServerAddress;
	unsigned short mServerPort;

	sf::Clock mClock;
	sf::Time mClockOffset;	//server.clock - this.clock

};