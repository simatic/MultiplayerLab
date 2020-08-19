#pragma once
#include <ServerWorld.h>
#include <SFML/Network.hpp>
#include <ClientData.h>
#include <stack>

class GameServer
{
public:
	GameServer();

	void processWaitingPackets();
	void processReceivedPacket(sf::Packet& packet, sf::IpAddress& remoteAddress, unsigned short remotePort);

	sf::Uint64 getNewEntityID();
	sf::Uint32 getNewClientID();
	ClientData& getClientFromID(sf::Uint32 id);

	void sendPing(ClientData& client);

private:
	ServerWorld mWorld;
	sf::Clock mClock;

	sf::UdpSocket mSocket;
	std::vector<ClientData> mClients;

	std::stack<sf::Uint64> mAvailableEntityIDs;
	std::stack<sf::Uint32> mAvailableClientIDs;

};