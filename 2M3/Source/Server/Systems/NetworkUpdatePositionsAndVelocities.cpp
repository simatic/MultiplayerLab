#include <Server/Systems/NetworkUpdatePositionsAndVelocities.h>
#include <Common/Managers/GameManager.h>
#include <Server/Modules/ServerNetworkModule.h>
#include <Server/Server.h>
#include <Network/InputPacket.h>
#include <Common/Network/SetTransformPacket.h>
#include <Common/Network/SetSpeedPacket.h>

NetworkUpdatePositionsAndVelocities::NetworkUpdatePositionsAndVelocities(GameManager* const gameManager, ServerNetworkModule* const networkModule) :
        ServerNetworkSystem<Transform, Kinematics>(gameManager, networkModule)
{}

void NetworkUpdatePositionsAndVelocities::update(const sf::Time& dt) {
    timer += dt.asSeconds();
    if(timer < 0.02) {
        return;
    }
    timer -= 0.02;
   
    auto& clients = networkModule->getServer().getClients();

    for(const auto& entity : entities) {
        auto transform = entity->getComponent<Transform>();
        auto kinematics = entity->getComponent<Kinematics>();
        for(const auto& client : clients) {
            auto transformPacket = networkModule->create<SetTransformPacket>(entity->getID(), transform);
            auto velocityPacket = networkModule->create<SetVelocityPacket>(entity->getID(), kinematics->velocity);
            client->send(std::move(transformPacket));
            client->send(std::move(velocityPacket));
        }
    }
}