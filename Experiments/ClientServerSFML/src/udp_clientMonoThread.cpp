#ifdef UNIX
// pour gethostname
#include <unistd.h>
// pour getpid()
#include <sys/types.h>
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
#include <utility>

#include <SFML/Network.hpp>
#include <SFML/System.hpp> // Pour sf::sleep

#include "common.h"

using namespace std;

constexpr int DEFAULT_NB_MESSAGES(60);

static int32_t clientId;
static sf::Clock myClock;

void processWaitingPackets(sf::UdpSocket &socket)
{
	do {
		// We try to see if there is a message to process
		sf::Packet packet;
		optional<sf::IpAddress> remoteAddress;
		unsigned short remotePort;
		if (socket.receive(packet, remoteAddress, remotePort) == sf::Socket::Status::NotReady)
			break;

		// We process the message
		int32_t msgType;
		packet >> msgType;
		switch (auto messageType = static_cast<ServerMsgType>(msgType))
		{
			case ServerMsgType::Broadcast:
			{
				int32_t senderClientId;
				int32_t i;
				int64_t sendElapsed;
				packet >> senderClientId >> i >> sendElapsed;
				int64_t receiveElapsed = myClock.getElapsedTime().asMicroseconds();
				cout << (clientId == senderClientId ? "[Myself] " : "") << "Client '" << senderClientId 
					<< "' has broadcast (thanks to server) integer: " << i
					<< " taking " << receiveElapsed - sendElapsed << " microseconds" << endl;
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
				int32_t aClientId;
				string aClientName;
				packet >> aClientId >> aClientName;
				cout << "New client " << aClientName << " with clientId " << aClientId << endl;
				break;
			}
			default:
				cerr << "ERROR: Received from server unknown messageType:'" << to_underlying(messageType) << "'" << endl;
				exit(EXIT_FAILURE);
		}

	}
	while (true); // We exit this loop thanks to break instruction when ((status == sf::Socket::NotReady) || (status == sf::Socket::Disconnected))
}

void sendPacket(sf::UdpSocket &socket, sf::Packet &packet, const sf::IpAddress &recipient, unsigned short remotePort)
{
	sf::Socket::Status status = socket.send(packet, recipient, remotePort);
	if (status != sf::Socket::Status::Done) {
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
	auto recipient = sf::IpAddress::resolve(host);
	if (!recipient.has_value()) {
		cerr << "Error: Cannot determine IP address of host " << host << endl;
		return EXIT_FAILURE;
	}

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
	if (gethostname(hostname, sizeof(hostname)) < 0) {
		cerr << "Error when executing gethostname()" << endl;
		exit(EXIT_FAILURE);
	}
	ostringstream buffer;
	buffer << hostname << ":" << localPort << "(pid=" << GETPID() << ")";
	string clientName(buffer.str());

	std::cout << "My clientName is " << clientName << endl;
	std::cout << "I will work with server at " << host << ":" << remotePort << endl;

	sf::UdpSocket socket;
	if (socket.bind(localPort) != sf::Socket::Status::Done)
	{
		cerr << "Error: Problem during binding to local port " << localPort << " : Another process is probably already using this port" << endl;
		return EXIT_FAILURE;
	}
	socket.setBlocking(false);

	// Send our clientId to the server
	sf::Packet packet;
	packet << to_underlying(ClientMsgType::ClientIdRequest) << clientName;
	sendPacket(socket, packet, recipient.value(), remotePort);
	sf::sleep(sf::milliseconds(1000)); // To give time to server to answer, so that we set clientId
	processWaitingPackets(socket);

	// Ask server to broadcast integers.
	cout << "Broadcasting " << nbMessages << " messages (at frequency of 1 message per second)" << endl;
	for (int i = 1; i <= nbMessages; ++i) {
		cout << "Request to broadcast integer: " << i << endl;
		packet.clear();
		// NB : "Security leak" as the client sends its clientId (it could send another clientId
		packet << to_underlying(ClientMsgType::IntToBroadcast) << clientId << i << myClock.getElapsedTime().asMicroseconds();
		sendPacket(socket, packet, recipient.value(), remotePort);
		processWaitingPackets(socket);
		sf::sleep(sf::milliseconds(1000));
	}
	cout << "Done" << endl;
}
