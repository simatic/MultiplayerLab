#include "Server/Server.h"
#include "Server/Interface.h"

Server::Server(const std::string& ip, unsigned short port): networkHandler(ip, port), networkThread(networkHandler) {}

void Server::run() {
    while(networkHandler.isRunning())
    {
        std::this_thread::yield();
    } // wait for network to be shutdown
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

ServerNetworkHandler& Server::getNetworkHandler() {
    return networkHandler;
}

bool Server::isReady() {
    return networkThread.isReady();
}
