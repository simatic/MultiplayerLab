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
#include <thread>
#include <memory>
#include <utility>

#include <SFML/Network.hpp>
#include <SFML/System.hpp> // Pour sf::sleep

#include "common.h"

using namespace std;

constexpr int DEFAULT_NB_MESSAGES(60);

static int32_t clientId;

[[noreturn]] void messageManager(const shared_ptr<sf::TcpSocket> &pSocket) {
	sf::Packet packet;
	while (pSocket->receive(packet) == sf::Socket::Status::Done) {
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
			cerr << "ERROR: Received from server unknown messageType:'" << to_underlying(messageType) << "'" << endl;
			exit(EXIT_FAILURE);
		}
	}

	cerr << "Server disconnected ==> Client terminates" << endl;
	exit(EXIT_FAILURE);
}

void usage(string_view progName)
{
	cout << progName << " [nbMessages [host [port]]]" << endl;
	cout << "\tnbMessages: Number of messages to send to server for broadcast (" << DEFAULT_NB_MESSAGES << " by default)" << endl;
	cout << "\thost: Host where server is located (" << DEFAULT_HOST << " by default)" << endl;
	cout << "\tport: Port on which to contact server (" << DEFAULT_PORT << " by default)" << endl;
}

int main(int argc, char* argv[])
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

	string_view host(DEFAULT_HOST);
	if (argc > 2)
	{
		host = argv[2];
	}

	int nbMessages(DEFAULT_NB_MESSAGES);
	if (argc > 1)
	{
		istringstream iss(argv[1]);
		iss >> nbMessages;
		if (!iss)
		{
			usage(argv[0]);
			return EXIT_FAILURE;
		}
	}

	char hostname[64];
	if (gethostname(hostname, sizeof(hostname)) < 0) {
		cerr << "Error when executing gethostname()" << endl;
		return EXIT_FAILURE;
	}
	ostringstream buffer;
	buffer << "host=" << hostname << "/pid=" << GETPID();
	string clientName(buffer.str());

	cout << "My clientName is '" << clientName << "'" << endl;

	auto pSocket = make_shared<sf::TcpSocket>();
	cout << "Connect to " << host << ":" << port << endl;
	auto hostIpAddress = sf::IpAddress::resolve(host);
	if (!hostIpAddress.has_value()) {
		cerr << "Error: Cannot determine IP address of host " << host << endl;
		return EXIT_FAILURE;
	}
	if (pSocket->connect(hostIpAddress.value(), port) != sf::Socket::Status::Done) {
		cerr << "Error: Problem during connect:" << endl;
		cerr << "\t- server is probably not running;" << endl;
		cerr << "\t- or the server you are trying to connect to is *not* the right one;" << endl;
		cerr << "\t- or the port you are trying to connect to is *not* the one listened by the server." << endl;
		exit(EXIT_FAILURE);
	}

	// Fork thread to handle messages received on this connection
	jthread t(messageManager, pSocket);

	// Send our clientId to the server
	sf::Packet packet;
	packet << to_underlying(ClientMsgType::ClientIdRequest);
	if (pSocket->send(packet) != sf::Socket::Status::Done) {
		cerr << "Error: Problem during send of CLIENT_ID" << endl;
		exit(EXIT_FAILURE);
	}
	sf::sleep(sf::milliseconds(1000));

	// Ask server to broadcast integers.
	cout << "Broadcasting " << nbMessages << " messages (at frequency of 1 message per second)" << endl;
	for (int i = 1; i <= nbMessages; ++i) {
		cout << "Request to broadcast integer: " << i << endl;
		packet.clear();
		packet << to_underlying(ClientMsgType::IntToBroadcast) << i;
		if (pSocket->send(packet) != sf::Socket::Status::Done) {
			cerr << "Error: Problem during send of Client::IntToBroadcast" << endl;
			exit(EXIT_FAILURE);
		}

		sf::sleep(sf::milliseconds(1000));
	}
	cout << "Done" << endl;
}
