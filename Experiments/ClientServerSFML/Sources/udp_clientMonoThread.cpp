#ifdef UNIX
// pour gethostname
#include <unistd.h>
// pour getpid()
#include <sys/types.h>
#include <unistd.h>
#define GETPID getpid
#else
// pour gethostname (NB : requiert librairie Ws2_32.lib ou DLL Ws2_32.dll
#include <winsock2.h>
// pour _getpid ()
#include <process.h>  
#define GETPID _getpid
#endif

#include <iostream>
#include <sstream>

#include <SFML/Network.hpp>
#include <SFML/System.hpp> // Pour sf::sleep

#include "common.h"

using namespace std;

const int DEFAULT_NB_MESSAGES(60);

static sf::Int32 clientId;

void processWaitingPackets(sf::UdpSocket &socket)
{
	sf::Socket::Status status;
	do
	{
		// We try to see if there is a message to process
		sf::Packet packet;
		sf::IpAddress remoteAddress;
		unsigned short remotePort;
		status = socket.receive(packet, remoteAddress, remotePort);
		if (status == sf::Socket::NotReady)
			break;

		// We process the message
		sf::Int32 msgType;
		packet >> msgType;
		ServerMsgType messageType = static_cast<ServerMsgType>(msgType);
		switch (messageType)
		{
			case ServerMsgType::Broadcast:
			{
				sf::Int32 senderClientId;
				sf::Int32 i;
				packet >> senderClientId >> i;
				cout << "Client '" << senderClientId << "' has broadcast (thanks to server) integer: " << i << endl;
				break;
			}
			case ServerMsgType::ClientIdResponse:
			{
				packet >> clientId;
				cout << "Received my clientId from server. It is '" << clientId << "'" << endl;
				break;
			}
			case ServerMsgType::newClient:
			{
				sf::Int32 aClientId;
				string aClientName;
				packet >> aClientId >> aClientName;
				cout << "New client " << aClientName << " with clientId " << aClientId << endl;
				break;
			}
			default:
				cerr << "ERROR: Received from server unknown messageType:'" << static_cast<int>(messageType) << "'" << endl;
				exit(EXIT_FAILURE);
		}

	}
	while (true); // We exit this loop thanks to break instruction when ((status == sf::Socket::NotReady) || (status == sf::Socket::Disconnected))
}

void sendPacket(sf::UdpSocket &socket, sf::Packet &packet, sf::IpAddress &recipient, unsigned short remotePort)
{
	sf::Socket::Status status = socket.send(packet, recipient, remotePort);
	if (status != sf::Socket::Done) {
		cerr << "Error: Problem during send of a packet" << endl;
		exit(EXIT_FAILURE);
	}
}

void usage(string_view progName)
{
	cout << progName << "[localPort [nbMessages [host [remotePort]]]]" << endl;
	cout << "\tlocalPort: Port on which client waits fo UDP messages (" << DEFAULT_PORT << " by default)" << endl;
	cout << "\tnbMessages: Number of messages to send to server for broadcast (" << DEFAULT_NB_MESSAGES << " by default)" << endl;
	cout << "\thost: Host where server is located (" << DEFAULT_HOST << " by default)" << endl;
	cout << "\tremotePort: Port on which to contact server (" << DEFAULT_PORT << " by default)" << endl;
}

int main(int argc, char *argv[])
{
	if (argc > 5)
	{
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	unsigned short remotePort(DEFAULT_PORT);
	if (argc > 4)
	{
		istringstream iss(argv[4]);
		iss >> remotePort;
		if (!iss)
		{
			usage(argv[0]);
			return EXIT_FAILURE;
		}
	}

	string_view host("localhost");
	if (argc > 3)
	{
		host = argv[3];
	}
	sf::IpAddress recipient(host.data());

	int nbMessages(DEFAULT_NB_MESSAGES);
	if (argc > 2)
	{
		std::istringstream iss(argv[2]);
		iss >> nbMessages;
		if (!iss)
		{
			usage(argv[0]);
			return EXIT_FAILURE;
		}
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

	char hostname[64];
	int rc = gethostname(hostname, sizeof(hostname));
	if (rc < 0) {
		cerr << "Erreur lors de l'execution de gethostname()" << endl;
		exit(EXIT_FAILURE);
	}
	ostringstream buffer;
	buffer << hostname << ":" << localPort << "(pid=" << GETPID() << ")";
	string clientName(buffer.str());

	std::cout << "My clientName is " << clientName << endl;
	std::cout << "I will work with server at " << host << ":" << remotePort << endl;

	sf::UdpSocket socket;
	sf::Socket::Status status = socket.bind(localPort);
	if (status != sf::Socket::Done)
	{
		cerr << "Error: Problem during binding to local port " << localPort << " (status = " << status << ") : Another process is probably already using this port" << endl;
		return EXIT_FAILURE;
	}
	socket.setBlocking(false);

	// Send our clientId to the server
	sf::Packet packet;
	packet << static_cast<int>(ClientMsgType::ClientIdRequest) << clientName;
	sendPacket(socket, packet, recipient, remotePort);
	sf::sleep(sf::milliseconds(1000));
	processWaitingPackets(socket);

	// Ask server to broadcast integers.
	cout << "Broadcasting " << nbMessages << " messages (at frequency of 1 message per second)" << endl;
	for (int i = 1; i <= nbMessages; ++i) {
		cout << "Request to broadcast integer: " << i << endl;
		sf::Packet packet;
		// NB : "Security leak" as the client sends its clientId (it could send another clientId
		packet << static_cast<int>(ClientMsgType::IntToBroadcast) << clientId << i;
		sendPacket(socket, packet, recipient, remotePort);
		processWaitingPackets(socket);
		sf::sleep(sf::milliseconds(1000));
	}
	cout << "Done" << endl;
}
