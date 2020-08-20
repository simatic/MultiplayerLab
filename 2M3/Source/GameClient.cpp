#include <GameClient.h>
#include <NetworkCommon.h>

GameClient::GameClient()
	: mSocket()
{
	mAddress = sf::IpAddress::getPublicAddress();
	mPort = sf::Socket::AnyPort;
}

sf::IpAddress GameClient::getAddress()
{
	return mAddress;
}

unsigned short GameClient::getPort()
{
	return mPort;
}

sf::Socket::Status GameClient::bindSocket()
{
	return mSocket.bind(mPort);
}

void GameClient::processWaitingPackets()
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

void GameClient::processReceivedPacket(sf::Packet& packet, sf::IpAddress& remoteAddress, unsigned short remotePort)
{
	ServerMsgType messageType;
	packet >> messageType;

	switch (messageType)
	{
	case ServerMsgType::CarUpdate:
	{
		break;
	}
	case ServerMsgType::ClientIdResponse:
	{
		sf::Time timeSent;
		packet >> mID >> timeSent;
		mClockOffset = timeSent - mClock.getElapsedTime();

		break;
	}
	case ServerMsgType::Collision:
	{
		break;
	}
	case ServerMsgType::GameEnd:
	{
		break;
	}
	case ServerMsgType::ObjectCreation:
	{
		break;
	}
	case ServerMsgType::ObjectDestruction:
	{
		break;
	}
	case ServerMsgType::PingRequest:
	{
		sf::Time elapsed;
		packet >> elapsed;

		sf::Packet toSend;
		toSend << ClientMsgType::PingResponse << mID << elapsed;
		mSocket.send(toSend, mServerAddress, mServerPort);

		break;
	}
	default:
		break;
	}
}

void GameClient::sendCarsInputs(const std::vector<Player*>& players)
{
	for (auto& player : players)
	{
		sf::Packet toSend;
		Car* car = player->getCar();
		toSend << ClientMsgType::Input << car->getID() << car->getSavedInputs();

		mSocket.send(toSend, mServerAddress, mServerPort);
	}
}
