#include "Server/Server.h"
#include "Server/Interface.h"

Server::Server(const std::string& ip, unsigned short port): networkHandler(ip, port), networkThread(networkHandler) {}

void Server::run() {
    {
        auto interface = Interface(networkHandler);
    } // wait for interface to "die" (ie. closing the window)
}

bool Server::isRunning() {
    return networkHandler.isRunning();
}

void Server::stop() {
    networkHandler.killNetworkThread();
}

Server::~Server() {
    networkHandler.killNetworkThread();
}
