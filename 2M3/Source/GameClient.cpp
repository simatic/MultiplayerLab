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
	sf::Socket::Status status;
	do
	{
		// We try to see if there is a message to process
		sf::Packet packet;
		sf::IpAddress remoteAddress;
		unsigned short remotePort;
		status = socket.receive(packet, remoteAddress, remotePort);
		if (status == sf::Socket::NotReady || status == sf::Socket::Disconnected)
			break;

		// We process the message
		processReceivedPacket(socket, packet, remoteAddress, remotePort);

	} while (true); // We exit this loop thanks to break instruction when ((status == sf::Socket::NotReady) || (status == sf::Socket::Disconnected))
}

void GameClient::processReceivedPacket(sf::UdpSocket& socket, sf::Packet& packet, sf::IpAddress& remoteAddress, unsigned short remotePort)
{
	sf::Uint32 msgType;
	packet >> msgType;
	ServerMsgType messageType = static_cast<ServerMsgType>(msgType);
	switch (messageType)
	{
	case ServerMsgType::CarUpdate:
	{
		break;
	}
	case ServerMsgType::ClientIdResponse:
	{
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
		sf::Int64 elapsed;
		packet >> elapsed;

		sf::Packet toSend;
		toSend << static_cast<sf::Uint32>(ClientMsgType::PingResponse) << mID << elapsed;
		socket.send(toSend, mServerAddress, mServerPort);

		break;
	}
	default:
		break;
	}
}
