#pragma once
#include <SFML/Network.hpp>
#include <Player.h>
#include <World.h>

class GameClient
{
public:

	GameClient();

	sf::IpAddress getAddress();
	unsigned short getPort();
	sf::IpAddress getServerAddress();

	void setServerAddress(sf::IpAddress serverAddress);
	void setServerPort(unsigned short port);
	void setSocketBlocking(bool isBlocking);

	sf::Socket::Status bindSocket();

	bool ownsCar(sf::Uint64 id, World& world);

	void sendPacket(sf::Packet packet, sf::IpAddress remoteAddress, unsigned short remotePort);	

	void processWaitingPackets(World& world);
	void processReceivedPacket(sf::Packet& packet, sf::IpAddress& remoteAddress, unsigned short remotePort, World& world);

	void sendCarsInputs(const std::vector<Player*>& players);

private:
	sf::Uint32 mID;
	sf::IpAddress mAddress;
	unsigned short mPort;
	sf::UdpSocket mSocket;

	sf::IpAddress mServerAddress;
	unsigned short mServerPort;

	sf::Clock mClock;
	sf::Time mClockOffset;	//server.clock - this.clock - delay

};