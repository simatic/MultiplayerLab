#pragma once
#include <ServerWorld.h>
#include <SFML/Network.hpp>
#include <ClientData.h>
#include <stack>
#include <SFML/System/Thread.hpp>

class GameServer
{
public:
	GameServer();

	const sf::IpAddress& getAdress();
	const unsigned short& getPort();
	
	void setSocketBlocking(bool isBlocking);

	sf::Socket::Status bindPort();

	void run();
	void start(sf::RenderWindow* window);
	void stop();

	void processWaitingPackets();
	void processReceivedPacket(sf::Packet& packet, sf::IpAddress& remoteAddress, unsigned short remotePort);

	sf::Uint64 getNewEntityID();
	sf::Uint32 getNewClientID();
	ClientData& getClientFromID(sf::Uint32 id);

	void sendPing(ClientData& client);

private:
	sf::RenderWindow* mHostWindow;
	ServerWorld mWorld;
	sf::Clock mClock;

	sf::IpAddress	mAdress;
	unsigned short  mPort;
	sf::UdpSocket mSocket;
	std::vector<ClientData> mClients;

	std::stack<sf::Uint64> mAvailableEntityIDs;
	std::stack<sf::Uint32> mAvailableClientIDs;

	sf::Thread mThread;
	bool mStopThread;

};