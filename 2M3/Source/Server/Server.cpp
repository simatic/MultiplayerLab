#include <Transform.h>
#include <Network/SetTransformPacket.h>
#include "Server/Server.h"
#include "Server/Interface.h"
#include "Common/Network/AddEntityPacket.h"

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
    if(event.type == NetworkEvent::Connected) {
        std::cout << "New client on port " << client.port << "!" << std::endl;
        auto ID = getNetworkModule().getNewNetworkID();
        auto entity = Prefab::createCar(false);
        auto* entTransform = entity->getComponent<Transform>();
        entTransform->position = sf::Vector2f(client.id*150, 0);
        game.addEntityWithID(entity, ID);

        //packet pour le client qui arrive
        auto hereIsYourEntityPacket = getNetworkHandler().create<AddEntityPacket>(Prefab::Type::playableCar, ID);
        client.send(std::move(hereIsYourEntityPacket));
        client.send(getNetworkHandler().create<SetTransformPacket>(entity->getID(), entTransform->position.x, entTransform->position.y, entTransform->rotation));

        //packets pour notifier les autres clients déjà présents
        for(auto& otherClient : getNetworkHandler().getClients()) {
            if(client.id != otherClient->id) {
                auto packet = getNetworkHandler().create<AddEntityPacket>(Prefab::Type::car, ID);
                otherClient->send(std::move(packet));
                otherClient->send(getNetworkHandler().create<SetTransformPacket>(entity->getID(), entTransform->position.x, entTransform->position.y, entTransform->rotation));
            }
        }
    }
}
