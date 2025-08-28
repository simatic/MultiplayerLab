#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>
#include <utility>
#include <vector>

#include "Client.h"
#include "common.h"

using namespace std;

// TODO Missing cleaning of vecClient for clients which did not contact server
// for a long time
static vector<Client> vecClient;

void processReceivedPacket(sf::UdpSocket &socket, sf::Packet &packet,
                           const sf::IpAddress &remoteAddress,
                           unsigned short remotePort) {
  int32_t msgType;
  packet >> msgType;
  switch (auto messageType = static_cast<ClientMsgType>(msgType)) {
    case ClientMsgType::ClientIdRequest: {
      string clientName;
      packet >> clientName;
      vecClient.emplace_back(clientName, remoteAddress, remotePort);
      auto clientId = static_cast<int32_t>(vecClient.size()) - 1;
      cout << "Client " << clientName
           << " is \"connected\" and given the clientId " << clientId << endl;

      sf::Packet packetToSend;
      packetToSend << to_underlying(ServerMsgType::ClientIdResponse)
                   << clientId;
      if (socket.send(packetToSend, remoteAddress, remotePort) !=
          sf::Socket::Status::Done) {
        cerr << "Error: Failed to send ServerMsgType::ClientIdResponse" << endl;
        exit(1);
      }

      packetToSend.clear();
      packetToSend << to_underlying(ServerMsgType::newClient) << clientId
                   << clientName;
      for (auto &client : vecClient)
        if (socket.send(packetToSend, client.getAddress(), client.getPort()) !=
            sf::Socket::Status::Done) {
          cerr << "Error: Failed to send ServerMsgType::newClient" << endl;
          exit(1);
        }
      break;
    }
    case ClientMsgType::IntToBroadcast: {
      int32_t clientId;
      int32_t i;
      int64_t sendElapsed;
      // TODO : Ideally, we should find CLientId by going through vecClient
      //        to find the rank of this client.
      packet >> clientId >> i >> sendElapsed;
      sf::Packet packetToSend;
      packetToSend << to_underlying(ServerMsgType::Broadcast) << clientId << i
                   << sendElapsed;
      for (auto &client : vecClient)
        if (socket.send(packetToSend, client.getAddress(), client.getPort()) !=
            sf::Socket::Status::Done) {
          cerr << "Error: Failed to send ServerMsgType::Broadcast" << endl;
          exit(1);
        }
      break;
    }
    default:
      std::cerr << "Received unknown message type '"
                << to_underlying(messageType) << "' from a client" << endl;
      exit(1);
  }
}

void usage(string_view progName) {
  cout << progName << " [port]" << endl;
  cout << "\tport: Port on which to listen (" << DEFAULT_PORT << " by default)"
       << endl;
}

int main(int argc, char **argv) {
  if (argc > 2) {
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  unsigned short localPort(DEFAULT_PORT);
  if (argc > 1) {
    istringstream iss(argv[1]);
    iss >> localPort;
    if (!iss) {
      usage(argv[0]);
      return EXIT_FAILURE;
    }
  }

  // Bind to port
  sf::UdpSocket socket;
  if (socket.bind(localPort) != sf::Socket::Status::Done) {
    cerr << "Error: Problem during binding to local port " << localPort
         << " : Another process is probably already using this port" << endl;
    return EXIT_FAILURE;
  }

  // Wait for messages on this socket
  std::cerr << "Waiting for messages on port " << localPort << std::endl;
  while (true) {
    sf::Packet packet;
    std::optional<sf::IpAddress> remoteAddress;
    unsigned short remotePort;

    if (socket.receive(packet, remoteAddress, remotePort) !=
        sf::Socket::Status::Done) {
      cerr << "Error during receive)" << endl;
      return EXIT_FAILURE;
    }

    processReceivedPacket(socket, packet, remoteAddress.value(), remotePort);
  }
}
