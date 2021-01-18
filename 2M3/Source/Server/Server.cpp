#include "Server/Server.h"
#include "Server/Interface.h"

Server::Server(const std::string& ip, unsigned short port): networkModule(std::make_unique<ServerNetworkHandler>(ip, port)) {}

void Server::run() {
    {
        auto networkHandler = dynamic_cast<ServerNetworkHandler*>(networkModule.getNetwork().get());
        auto interface = Interface(*networkHandler);
    } // wait for interface to "die" (ie. closing the window)
}

bool Server::isRunning() {
    auto networkHandler = dynamic_cast<ServerNetworkHandler*>(networkModule.getNetwork().get());
    return networkHandler->isRunning();
}

void Server::stop() {
    auto networkHandler = dynamic_cast<ServerNetworkHandler*>(networkModule.getNetwork().get());
    networkHandler->killNetworkThread();
}

Server::~Server() {
    auto networkHandler = dynamic_cast<ServerNetworkHandler*>(networkModule.getNetwork().get());
    networkHandler->killNetworkThread();
}
