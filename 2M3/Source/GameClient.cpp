#include <GameClient.h>
#include <NetworkCommon.h>
#include <iostream>

GameClient::GameClient()
	: mSocket()
{
	mPort = sf::Socket::AnyPort;
	bindSocket();
}

sf::IpAddress GameClient::getAddress()
{
	return mAddress;
}

unsigned short GameClient::getPort()
{
	return mPort;
}

void GameClient::setServerAddress(sf::IpAddress serverAddress)
{
	mServerAddress = serverAddress;
}

sf::Socket::Status GameClient::bindSocket()
{
	return mSocket.bind(mPort);
}

void GameClient::sendPacket(sf::Packet packet, sf::IpAddress remoteAddress, unsigned short remotePort)
{
	if (mSocket.send(packet, remoteAddress, remotePort) != sf::Socket::Done)
	{
		std::cout << "Problem during the sending of a packet";
		std::exit(EXIT_FAILURE);
	}
}

void GameClient::processWaitingPackets(World& world)
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
		processReceivedPacket(packet, remoteAddress, remotePort, world);

	} while (true); // We exit this loop thanks to break instruction when ((status == sf::Socket::NotReady) || (status == sf::Socket::Disconnected))
}

void GameClient::processReceivedPacket(sf::Packet& packet, sf::IpAddress& remoteAddress, unsigned short remotePort, World& world)
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
		sf::Uint64 id1;
		sf::Uint64 id2;
		packet >> id1 >> id2;

		Entity* ent1 = world.getEntityFromId(id1);
		Entity* ent2 = world.getEntityFromId(id2);

		world.addCollision(ent1, ent2);

		break;
	}
	case ServerMsgType::GameEnd:
	{
		break;
	}
	case ServerMsgType::ObjectCreation:
	{
		EntityStruct baseEnt;
		packet >> baseEnt;

		switch (baseEnt.entityType)
		{
		case Entity::Type::CarType:
		{
			break;
		}
		case Entity::Type::PickUpType:
		{
			break;
		}
		case Entity::Type::ProjectileType:
		{
			break;
		}
		default:
			break;
		}

		break;
	}
	case ServerMsgType::ObjectDestruction:
	{
		sf::Uint64 id;
		packet >> id;

		world.getEntityFromId(id)->remove();

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
