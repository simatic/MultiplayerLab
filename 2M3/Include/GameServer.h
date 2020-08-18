#pragma once
#include <World.h>
#include <SFML/Network.hpp>
#include <ClientData.h>

class GameServer
{
public:


	void processWaitingPackets(sf::UdpSocket& socket);
	void processReceivedPacket(sf::UdpSocket& socket, sf::Packet& packet, sf::IpAddress& remoteAddress, unsigned short remotePort);

private:
	World mWorld;
	sf::Clock mClock;

	sf::UdpSocket mSocket;
	std::vector<ClientData> mClients;

};