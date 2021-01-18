#include "Server/Server.h"
#include "Server/Interface.h"

Server::Server(const std::string& ip, unsigned short port): game()
{
    game.setNetworkModule<ServerNetworkModule>(ip, port);
    gameThread = std::thread([&]() { runGame(); });
    getNetworkHandler().registerListener(this);
}

void Server::runGame() {
    while(!isReady()) {
        std::this_thread::yield();
    }
    // TODO: precise ticks
    sf::Clock clock{};
    while(isRunning())
    {
        auto dt = clock.restart();
        game.update(dt);
        std::this_thread::yield();
    } // wait for network to be shutdown
}

void Server::run() {
    while(isRunning())
    {
        std::this_thread::yield();
    } // wait for network to be shutdown
    gameThread.join();
}

bool Server::isRunning() {
    return getNetworkHandler().isRunning();
}

void Server::stop() {
    getNetworkHandler().killNetworkThread();
}

Server::~Server() {
    getNetworkHandler().killNetworkThread();
}

ServerNetworkHandler& Server::getNetworkHandler() {
    return *dynamic_cast<ServerNetworkHandler*>(getNetworkModule().getNetwork().get());
}

ServerNetworkModule& Server::getNetworkModule() {
    return *dynamic_cast<ServerNetworkModule*>(game.getNetworkModule());
}

bool Server::isReady() {
    return getNetworkModule().getNetworkThread().isReady();
}

void Server::onEvent(const UdpClient& client, NetworkEvent::Event event) {
    // TODO
    if(event.type == NetworkEvent::Connected) {
        std::cout << "New client on port " << client.port << "!" << std::endl;
    }
}
