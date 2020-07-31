#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <mutex>
#include <memory>

#include <SFML/Network.hpp>

#include "common.h"
#include "Client.h"

using namespace std;

// TODO Missing cleaning of vecClient for clients which did not contact server for a long time
static vector<Client> vecClient;

void processReceivedPacket(sf::UdpSocket &socket, sf::Packet &packet, sf::IpAddress &remoteAddress, unsigned short remotePort)
{
	sf::Int32 msgType;
	packet >> msgType;
	ClientMsgType messageType = static_cast<ClientMsgType>(msgType);
	switch (messageType)
	{
		case ClientMsgType::ClientIdRequest:
		{	
			string clientName;
			packet >> clientName;
			vecClient.push_back(Client(clientName, remoteAddress, remotePort));
			sf::Int32 clientId = vecClient.size() - 1;
			cout << "Client " << clientName << " is \"connected\" and given the clientId " << clientId << endl;

			sf::Packet packetToSend;
			packetToSend << static_cast<sf::Int32>(ServerMsgType::ClientIdResponse) << clientId;
			socket.send(packetToSend, remoteAddress, remotePort);

			packetToSend.clear();
			packetToSend << static_cast<sf::Int32>(ServerMsgType::newClient) << clientId << clientName;
			for (auto& client : vecClient)
				socket.send(packetToSend, remoteAddress, remotePort);
			break;
		}
		case ClientMsgType::IntToBroadcast:
		{
			sf::Int32 clientId;
			sf::Int32 i;
			sf::Int64 sendElapsed;
			// TODO : Ideally, we should find CLientId by going through vecClient
			//        to find the rank of this client.
			packet >> clientId >> i >> sendElapsed;
			cout << "Client '" << clientId << "' requests to broadcast int \"" << i 
				<< "\" at " << sendElapsed << " microseconds" << endl;
			sf::Packet packetToSend;
			packetToSend << static_cast<sf::Int32>(ServerMsgType::Broadcast) << clientId << i << sendElapsed;
			for (auto & client : vecClient)
				socket.send(packetToSend, client.getAddress(), client.getPort());
			break;
		}
		default:
			std::cerr << "Received unknown message type '" << static_cast<int>(messageType) << "' from a client" << endl;
			exit(1);
	}
}

void usage(string_view progName)
{
	cout << progName << " [port]" << endl;
	cout << "\tport: Port on which to listen (" << DEFAULT_PORT << " by default)" << endl;
}

int main(int argc, char** argv)
{
	if (argc > 2)
	{
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	unsigned short localPort(DEFAULT_PORT);
	if (argc > 1)
	{
		istringstream iss(argv[1]);
		iss >> localPort;
		if (!iss)
		{
			usage(argv[0]);
			return EXIT_FAILURE;
		}	
	}

	// Bind to port
	sf::UdpSocket socket;
	sf::Socket::Status status = socket.bind(localPort);
	if (status != sf::Socket::Done)
	{
		cerr << "Error: Problem during binding to local port " << localPort << " (status = " << status << ") : Another process is probably already using this port" << endl;
		return EXIT_FAILURE;
	}

	// Wait for messages on this socket
	std::cerr << "Waiting for messages on port " << localPort << std::endl;
	while (1) {
		sf::Packet packet;
		sf::IpAddress remoteAddress;
		unsigned short remotePort;
		status = socket.receive(packet, remoteAddress, remotePort);

		if (status != sf::Socket::Done)
		{
			cerr << "Error during receive (status = " << status << ")" << endl;
			return EXIT_FAILURE;
		}

		processReceivedPacket(socket, packet, remoteAddress, remotePort);
	}
}
