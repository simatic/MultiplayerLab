// MonoThread version of TCP server.
// The code is directly inspired from the example in
// https://www.sfml-dev.org/documentation/3.0.1/classsf_1_1SocketSelector.html
#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>
#include <utility>
#include <vector>

#include "common.h"

using namespace std;

// Returns false if receivePacket discovers client will not receive any more
// packets.
bool receivePacket(
    const vector<pair<int32_t, unique_ptr<sf::TcpSocket>>> &clients,
    const int32_t clientRank) {
  sf::Packet packet;
  if (clients[clientRank].second->receive(packet) != sf::Socket::Status::Done) {
    return false;
  }

  int32_t msgType;
  packet >> msgType;
  switch (auto messageType = static_cast<ClientMsgType>(msgType)) {
    case ClientMsgType::ClientIdRequest:
      cout << "Client '" << clients[clientRank].first
           << "' is connected and requesting its clientId" << endl;
      {
        sf::Packet packetToSend;
        packetToSend << to_underlying(ServerMsgType::ClientIdResponse)
                     << clients[clientRank].first;
        if (clients[clientRank].second->send(packetToSend) !=
            sf::Socket::Status::Done) {
          std::cerr << "Could not send message "
                    << to_underlying(ServerMsgType::ClientIdResponse)
                    << "to client " << clients[clientRank].first << endl;
          exit(1);
        }
      }
      break;
    case ClientMsgType::IntToBroadcast:
      int32_t i;
      packet >> i;
      cout << "Client '" << clients[clientRank].first
           << "' requests to broadcast int \"" << i << "\"" << endl;
      {
        sf::Packet packetToSend;
        packetToSend << to_underlying(ServerMsgType::Broadcast)
                     << clients[clientRank].first << i;

        for (const auto &[clientId, pSock] : clients)
          if (pSock->send(packetToSend) != sf::Socket::Status::Done) {
            std::cerr << "While \"broadcasting\" a message, could not send a "
                         "packet to client '"
                      << clientId << endl;
            exit(1);
          }
      }
      break;
    default:
      std::cerr << "Received unknown message type '"
                << to_underlying(messageType) << "' from client '"
                << clients[clientRank].first << "'" << endl;
      exit(1);
  }
  return true;
}

void testAllClientSockets(
    sf::SocketSelector &selector,
    vector<pair<int32_t, unique_ptr<sf::TcpSocket>>> &clients) {
  int clientRank = 0;
  while (clientRank < clients.size()) {
    if (selector.isReady(*(clients[clientRank].second)) &&
        !receivePacket(clients, clientRank)) {
      std::cerr << "Client '" << clients[clientRank].first << "' disconnected"
                << endl;
      selector.remove(*(clients[clientRank].second));
      clients[clientRank] = std::move(clients.back());
      clients.pop_back();
      continue;
    }
    clientRank += 1;
  }
}

void waitForDataOnAnySocket(sf::SocketSelector &selector,
                            sf::TcpListener &listener) {
  static int32_t nextClientId{0};
  // Create a list to store the future clients
  // - first field of the pair contains clientId of a client
  // - second field of the pair contains the socket to send messages to this
  // client.
  static vector<pair<int32_t, unique_ptr<sf::TcpSocket>>> clients;

  // Make the selector wait for data on any socket
  if (selector.wait()) {
    // Test the listener
    if (selector.isReady(listener)) {
      // The listener is ready: there is a pending connection
      auto pClient = make_unique<sf::TcpSocket>();
      if (listener.accept(*pClient) == sf::Socket::Status::Done) {
        // Add the new client to the selector so that we will
        // be notified when they send something
        selector.add(*(pClient.get()));

        // Add the new client to the clients list
        pair<int32_t, unique_ptr<sf::TcpSocket>> p{nextClientId,
                                                   std::move(pClient)};
        clients.emplace_back(std::move(p));
        nextClientId++;
      } else {
        std::cerr << "Error: Problem during accept" << std::endl;
        exit(1);
      }
    } else {
      // The listener socket is not ready, test all other sockets (the clients)
      testAllClientSockets(selector, clients);
    }
  }
}

void usage(string_view progName) {
  cout << progName << " [port]" << endl;
  cout << "\tport: Port on which to listen (" << DEFAULT_PORT << " by default)"
       << endl;
}

[[noreturn]] int main(int argc, char **argv) {
  if (argc > 2) {
    usage(argv[0]);
    exit(1);
  }

  unsigned short port(DEFAULT_PORT);
  if (argc > 1) {
    istringstream iss(argv[1]);
    iss >> port;
    if (!iss) {
      usage(argv[0]);
      exit(1);
    }
  }

  // Create a socket to listen to new connections
  sf::TcpListener listener;
  // Listen to port
  if (listener.listen(port) != sf::Socket::Status::Done) {
    std::cerr << "Error: Problem during listen (probably, there is already "
                 "another server running)"
              << std::endl;
    exit(1);
  }

  // Create a selector
  sf::SocketSelector selector;

  // Add the listener to the selector
  selector.add(listener);

  // Endless loop that waits for new connections
  while (true) {
    waitForDataOnAnySocket(selector, listener);
  }
}
