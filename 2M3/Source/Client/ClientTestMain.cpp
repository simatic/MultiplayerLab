#include <Common/Network/Constants.h>
#include <Common/Network/Buffer.h>
#include <SFML/Network.hpp>
#include <Client/Network/ClientNetworkHandler.h>
#include <Client/Network/ClientNetworkThread.h>

///
///
///     CODE DE TEST
///
///
///

int main(int argc, char** argv) {
    unsigned short remotePort = DEFAULT_PORT;
    if(argc > 1) {
        remotePort = atoi(argv[1]);
    }

    std::cout << "Remote port = " << remotePort << std::endl;

    ClientNetworkHandler network{};
    Buffer<Packet> buffer{};
    ClientNetworkThread thread{network, buffer, "localhost", remotePort};

    return 0;
}