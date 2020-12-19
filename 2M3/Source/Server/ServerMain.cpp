//
// Created by jglrxavpok on 15/10/2020.
//

#include "Server/Server.h"
#include <thread>
#include <iostream>
#include <Common/Network/Constants.h>
#include <sstream>
#include <Server/ServerNetworkHandler.h>
#include <Server/ServerNetworkThread.h>
#include <Server/Interface.h>
#include <Server/DelayCreation.h>

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

    {
        Server{"0.0.0.0", localPort}.run();
    }

    return 0;
}