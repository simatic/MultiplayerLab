#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <mutex>
#include <memory>
#include <utility>

#include <SFML/Network.hpp>

#include "common.h"

using namespace std;

static vector<shared_ptr<sf::TcpSocket>> vecClientSock;

static mutex mutexVec;

void clientManager(const shared_ptr<sf::TcpSocket> &pClientSock)
{
	size_t posClientSockInVector;
	{
		lock_guard guard(mutexVec);
		vecClientSock.push_back(pClientSock);
		posClientSockInVector = vecClientSock.size() - 1;
	}
	auto clientId = static_cast<int32_t>(posClientSockInVector);

	sf::Packet packet;
	while (pClientSock->receive(packet) == sf::Socket::Status::Done) {
		int32_t msgType;
		packet >> msgType;
		switch (auto messageType = static_cast<ClientMsgType>(msgType)) {
		case ClientMsgType::ClientIdRequest:
			cout << "Client '" << clientId << "' is connected and requesting its clientId" << endl;
			{
				sf::Packet packetToSend;
				packetToSend << to_underlying(ServerMsgType::ClientIdResponse) << clientId;
				if (pClientSock->send(packetToSend) != sf::Socket::Status::Done) {
					cerr << "Error: Problem during send of CLIENT_ID" << endl;
					exit(1);
				}
			}
			break;
		case ClientMsgType::IntToBroadcast:
			int32_t i;
			packet >> i;
			cout << "Client '" << clientId << "' requests to broadcast int \"" << i << "\"" << endl;
			{
				sf::Packet packetToSend;
				packetToSend << to_underlying(ServerMsgType::Broadcast) << clientId << i;

				lock_guard guard(mutexVec);
				for (const auto & sock : vecClientSock)
					if (sock->send(packetToSend) != sf::Socket::Status::Done) {
						cerr << "Error: Problem during broadcasting to clients" << endl;
						exit(EXIT_FAILURE);
					}
			}
			break;
		default:
			std::cerr << "Received unknown message type '" << to_underlying(messageType) << "' from client '" << clientId<< "'" << endl;
			exit(1);
		}
	}

	std::cerr << "Client '" << clientId << "' disconnected" << endl;
	{
		lock_guard guard(mutexVec);
		vecClientSock.erase(vecClientSock.begin() + posClientSockInVector);
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

	unsigned short port(DEFAULT_PORT);
	if (argc > 1)
	{
		istringstream iss(argv[1]);
		iss >> port;
		if (!iss)
		{
			usage(argv[0]);
			return EXIT_FAILURE;
		}	
	}

	sf::TcpListener listener;

	// Listen to port
	if (listener.listen(port) != sf::Socket::Status::Done) {
		std::cerr << "Error: Problem during listen (probably, there is already another server running)" << std::endl;
		exit(1);
	}

	// Wait for a new connection
	std::cerr << "Waiting for connections on port " << port << std::endl;
	while (true) {
		auto pClientSock = make_shared<sf::TcpSocket>();
		if (listener.accept(*pClientSock) != sf::Socket::Status::Done) {
			std::cerr << "Error: Problem during accept" << std::endl;
			exit(1);
		}

		// Fork thread to handle this client's connection
		jthread t(clientManager, pClientSock);
		t.detach();
	}

}
