#include "Server/Server.h"
#include "Server/Interface.h"

Server::Server(const std::string& ip, unsigned short port): networkHandler(ip, port), networkThread(networkHandler), game(),
    gameThread([&]() { runGame(); })
{}

void Server::runGame() {
    // TODO: precise ticks
    sf::Clock clock{};
    while(networkHandler.isRunning())
    {
        auto dt = clock.restart();
        game.update(dt);
        std::this_thread::yield();
    } // wait for network to be shutdown
}

void Server::run() {
    while(networkHandler.isRunning())
    {
        std::this_thread::yield();
    } // wait for network to be shutdown
    gameThread.join();
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
