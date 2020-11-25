//
// Created by jglrxavpok on 15/10/2020.
//

#include "Server/ServerMain.h"
#include <thread>
#include <iostream>
#include <Common/Constants.h>
#include <sstream>
#include <Server/ServerNetworkHandling.h>

void usage(char* executableName) {
    std::cerr << executableName << " <port>" << std::endl;
}

int main(int argc, char** argv) {
    unsigned short localPort(DEFAULT_PORT);
    if (argc > 1) {
        std::istringstream iss(argv[1]);
        iss >> localPort;
        if (!iss) {
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    std::thread network(networkThread, localPort);
    std::thread interface(interfaceThread, "0.0.0.0", localPort);
    interface.join();
    ServerNetworkHandling::killNetworkThread();

    network.join();
    return 0;
}