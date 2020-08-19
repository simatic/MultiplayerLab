#pragma once
#include <ServerWorld.h>
#include <SFML/Network.hpp>
#include <ClientData.h>
#include <stack>

class GameServer
{
public:


	void processWaitingPackets(sf::UdpSocket& socket);
	void processReceivedPacket(sf::UdpSocket& socket, sf::Packet& packet, sf::IpAddress& remoteAddress, unsigned short remotePort);

	sf::Uint64 getNewID();

private:
	ServerWorld mWorld;
	sf::Clock mClock;

	sf::UdpSocket mSocket;
	std::vector<ClientData> mClients;

	std::stack<sf::Uint64> mAvailableIDs;

};