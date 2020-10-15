//
// Created by jglrxavpok on 15/10/2020.
//

#include "Server/servermain.h"
#include <thread>

int main() {
    std::thread network(networkThread);
    std::thread interface(interfaceThread);

    network.join();
    interface.join();
    return 0;
}