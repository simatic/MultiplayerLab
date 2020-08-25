#include <GameServer.h>
#include <NetworkCommon.h>
#include <iostream>

GameServer::GameServer()
	: mWorld()
	, mClock()
	, mSocket()
	, mClients()
{
	mPort = ServerPort;
	mAdress	= sf::IpAddress::getLocalAddress();
	bindPort();
	mAvailableEntityIDs.push(1);
	mAvailableClientIDs.push(0);
}

void GameServer::processWaitingPackets()
{
	sf::Socket::Status status;
	do
	{
		// We try to see if there is a message to process
		sf::Packet packet;
		sf::IpAddress remoteAddress;
		unsigned short remotePort;
		status = mSocket.receive(packet, remoteAddress, remotePort);
		if (status == sf::Socket::NotReady || status == sf::Socket::Disconnected)
			break;

		// We process the message
		processReceivedPacket(packet, remoteAddress, remotePort);

	} while (true); // We exit this loop thanks to break instruction when ((status == sf::Socket::NotReady) || (status == sf::Socket::Disconnected))
}

void GameServer::processReceivedPacket(sf::Packet& packet, sf::IpAddress& remoteAddress, unsigned short remotePort)
{
	ClientMsgType messageType;
	packet >> messageType;

	switch (messageType)
	{
	case ClientMsgType::ClientIdRequest:
	{
		sf::Uint32 newID = getNewClientID();
		ClientData client = ClientData(newID, remoteAddress, remotePort);
		mClients.push_back(client);
		
		sf::Packet toSend;
		toSend << ServerMsgType::ClientIdResponse << newID << mClock.getElapsedTime();
		mSocket.send(toSend, remoteAddress, remotePort);

		break;
	}
	case ClientMsgType::Input:
	{
		sf::Uint64 id;
		Inputs inputs;
		packet >> id >> inputs;

		//mWorld.setCarInputs(id, inputs);

		break;
	}
	case ClientMsgType::PingResponse:
	{
		sf::Uint32 id;
		sf::Time timeSent;
		packet >> id >> timeSent;

		ClientData& client = getClientFromID(id);
		client.setDelay(sf::microseconds((mClock.getElapsedTime() - timeSent).asMicroseconds() / 2));

		break;
	}
	default:
		break;
	}
}

sf::Uint64 GameServer::getNewEntityID()
{
	sf::Uint64 id = mAvailableEntityIDs.top();
	mAvailableEntityIDs.pop();
	if (mAvailableEntityIDs.empty()) mAvailableEntityIDs.emplace(id + 1);
	return id;
}

sf::Uint32 GameServer::getNewClientID()
{
	sf::Uint32 id = mAvailableClientIDs.top();
	mAvailableClientIDs.pop();
	if (mAvailableClientIDs.empty()) mAvailableClientIDs.emplace(id + 1);
	return id;
}

ClientData& GameServer::getClientFromID(sf::Uint32 id)
{
	for (auto& client : mClients)
	{
		if (client.getID() == id) return client;
	}
	std::cerr << "Error: no client with such ID : " << id << std::endl;
	exit(EXIT_FAILURE);
}

void GameServer::sendPing(ClientData& client)
{
	sf::Packet packet;
	packet << ServerMsgType::PingRequest << mClock.getElapsedTime();
	mSocket.send(packet, client.getAddress(), client.getPort());
}

sf::Socket::Status GameServer::bindPort()
{
	return mSocket.bind(mPort);
}

const sf::IpAddress& GameServer::getAdress()
{
	const sf::IpAddress& t = mAdress;
	return t;
}

const unsigned short& GameServer::getPort()
{
	const unsigned short& t = mPort;
	return t;
}
