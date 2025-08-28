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

// Returns sf::Socket::Status::NotReady if there were no waiting packets or waiting packets which were processed
//      or sf::Socket::Status::Disconnected if the connection with server is lost
sf::Socket::Status processWaitingPackets(sf::TcpSocket &socket)
{
	sf::Socket::Status status;
	do {
		// We try to see if there is a message to process
		sf::Packet packet;
		status = socket.receive(packet);
		if ((status == sf::Socket::Status::NotReady) || (status == sf::Socket::Status::Disconnected))
			break;

		// We read the socket until we are sure that the message is complete
		while (status != sf::Socket::Status::Done)
			status = socket.receive(packet);

		// We process the message
		int32_t msgType;
		packet >> msgType;
		switch (auto messageType = static_cast<ServerMsgType>(msgType)) {
		case ServerMsgType::Broadcast:
		{
			int32_t senderClientId;
			int32_t i;
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
		default:
			cerr << "ERROR: Received from server unknown messageType:'" << std::to_underlying(messageType) << "'" << endl;
			exit(EXIT_FAILURE);
		}

	} while (true); // We exit this loop thanks to break instruction when ((status == sf::Socket::Status::NotReady) || (status == sf::Socket::Status::Disconnected))
	return status;
}

void sendPacket(sf::TcpSocket &socket, sf::Packet &packet)
{
	sf::Socket::Status status;
	using enum sf::Socket::Status;
	do {
		status = socket.send(packet);
		if ((status != Done) && (status != Partial)) {
			cerr << "Error: Problem during send of a packet" << endl;
			exit(EXIT_FAILURE);
		}
	} while (status == Partial);
}

void usage(string_view progName)
{
	cout << progName << " [nbMessages [host [port]]]" << endl;
	cout << "\tnbMessages: Number of messages to send to server for broadcast (" << DEFAULT_NB_MESSAGES << " by default)" << endl;
	cout << "\thost: Host where server is located (" << DEFAULT_HOST << " by default)" << endl;
	cout << "\tport: Port on which to contact server (" << DEFAULT_PORT << " by default)" << endl;
}

int main(int argc, char *argv[])
{
	if (argc > 4)
	{
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	unsigned short port(DEFAULT_PORT);
	if (argc > 3)
	{
		istringstream iss(argv[3]);
		iss >> port;
		if (!iss)
		{
			usage(argv[0]);
			return EXIT_FAILURE;
		}
	}

	string_view host("localhost");
	if (argc > 2)
	{
		host = argv[2];
	}

	int nbMessages(DEFAULT_NB_MESSAGES);
	if (argc > 1)
	{
		std::istringstream iss(argv[1]);
		iss >> nbMessages;
		if (!iss)
		{
			usage(argv[0]);
			return EXIT_FAILURE;
		}
	}

	char hostname[64];
	if (gethostname(hostname, sizeof(hostname)) < 0) {
		cerr << "Erreur when executing gethostname()" << endl;
		exit(EXIT_FAILURE);
	}
	ostringstream buffer;
	buffer << "host=" << hostname << "/pid=" << GETPID();
	string clientName(buffer.str());

	cout << "My clientName is '" << clientName << "'" << endl;

	sf::TcpSocket socket;
	cout << "Connect to " << host << ":" << port << endl;
	auto hostIpAddress = sf::IpAddress::resolve(host);
	if (!hostIpAddress.has_value()) {
		cerr << "Error: Cannot determine IP address of host " << host << endl;
		return EXIT_FAILURE;
	}
	if (socket.connect(hostIpAddress.value(), port) != sf::Socket::Status::Done) {
		cerr << "Error: Problem during connect" << endl;
		cerr << "\t- server is probably not running;" << endl;
		cerr << "\t- or the server you are trying to connect to is *not* the right one;" << endl;
		cerr << "\t- or the port you are trying to connect to is *not* the one listened by the server." << endl;
		return EXIT_FAILURE;
	}
	socket.setBlocking(false);

	// Send our clientId to the server
	sf::Packet packet;
	packet << std::to_underlying(ClientMsgType::ClientIdRequest);
	sendPacket(socket, packet);
	sf::sleep(sf::milliseconds(1000));

	// Ask server to broadcast integers.
	cout << "Broadcasting " << nbMessages << " messages (at frequency of 1 message per second)" << endl;
	for (int i = 1; i <= nbMessages; ++i) {
		cout << "Request to broadcast integer: " << i << endl;
		packet.clear();
		packet << std::to_underlying(ClientMsgType::IntToBroadcast) << i;
		sendPacket(socket, packet);
		if (processWaitingPackets(socket) == sf::Socket::Status::Disconnected) {
			cerr << "Connection with server is lost (server probably stopped)" << endl;
			break;
		}
		sf::sleep(sf::milliseconds(1000));
	}
	cout << "Done" << endl;
}
