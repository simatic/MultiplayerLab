#include <Transform.h>
#include <Network/SetTransformPacket.h>
#include <Network/WorldStatePacket.h>
#include <CopiableFromPrefab.h>
#include "Common/Components/Collider.h"
#include "Utility.h"
#include "Server/Server.h"
#include "Server/Interface.h"

Server::Server(const std::string& ip, unsigned short port): game()
{
    game.setNetworkModule<ServerNetworkModule>(ip, port);
    initGame();
    gameThread = std::thread([&]() { runGame(); });
    getNetworkHandler().registerListener(this);
}

void Server::createWall(float x, float y, float angle) {
    auto wall = Prefab::createWall(false);
    auto* transform = wall->getComponent<Transform>();
    auto& shape = wall->getComponent<Collider>()->shape;
    float wallLength = std::max(shape.getSize().x, shape.getSize().y);
    transform->position = {x * wallLength / 2.0f, y * wallLength / 2.0f};
    transform->rotation = angle;
    game.addEntityWithID(wall, getNetworkModule().getNewNetworkID());
}

void Server::initGame() {
    createWall(0, -1, 0);
    createWall(0, +1, 0);
    createWall(-1, 0, 90.0f);
    createWall(+1, 0, 90.0f);

    // TODO: add systems
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

void Server::makePlayerJoin(const UdpClient& client) {
    auto entityID = getNetworkModule().getNewNetworkID();
    auto entity = Prefab::createCar(false);
    auto* entTransform = entity->getComponent<Transform>();
    entTransform->position = sf::Vector2f(client.id*150, 0);
    playerEntityIDs[client.id] = entityID;
    game.addEntityWithID(entity, entityID);

    //packet pour le client qui arrive
    auto hereIsYourEntityPacket = getNetworkHandler().create<AddEntityPacket>(Prefab::Type::PlayableCar, entityID);
    client.send(std::move(hereIsYourEntityPacket));
    client.send(getNetworkHandler().create<SetTransformPacket>(entity->getID(), entTransform->position.x, entTransform->position.y, entTransform->rotation));

    //packets pour notifier les autres clients déjà présents
    for(auto& otherClient : getNetworkHandler().getClients()) {
        if(client.id != otherClient->id) {
            auto packet = getNetworkHandler().create<AddEntityPacket>(Prefab::Type::Car, entityID);
            otherClient->send(std::move(packet));
            otherClient->send(getNetworkHandler().create<SetTransformPacket>(entity->getID(), entTransform->position.x, entTransform->position.y, entTransform->rotation));
        }
    }
}

void Server::sendWorldStateTo(const UdpClient& client) {
    auto entities = game.getEntityList();
    // copy entities that can be copied to this vector
    std::vector<WorldStatePacket::EntityInformation> info{};
    for(const auto& entity : entities) {
        if(auto copiable = entity->getComponent<CopiableFromPrefab>()) {
            WorldStatePacket::EntityInformation entityInfo{};
            entityInfo.type = copiable->type;
            entityInfo.id = entity->getID();
            entityInfo.hasTransform = false;

            if(auto transform = entity->getComponent<Transform>()) {
                entityInfo.hasTransform = true;
                entityInfo.x = transform->position.x;
                entityInfo.y = transform->position.y;
                entityInfo.angle = transform->rotation;
            }

            info.push_back(entityInfo);
        }
    }

    // write informations to client
    auto worldStatePacket = getNetworkHandler().create<WorldStatePacket>(info);
    client.send(std::move(worldStatePacket));
}

void Server::onEvent(const UdpClient& client, NetworkEvent::Event event) {
    if(event.type == NetworkEvent::Connected) {
        std::cout << "New client on port " << client.port << "!" << std::endl;

        std::lock_guard lk(playerConnection);

        // FIXME: spot potentiel pour le setup du TP
        //  ie. envoi des informations des entités déjà présentes
        sendWorldStateTo(client);

        makePlayerJoin(client);
    }
}
