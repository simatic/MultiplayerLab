#include <Server/Systems/NetworkUpdatePositionsAndVelocities.h>
#include <Common/Managers/GameManager.h>
#include <Server/Modules/ServerNetworkModule.h>
#include <Server/Server.h>
#include <Network/InputPacket.h>
#include <Common/Network/SetTransformPacket.h>
#include <Common/Network/SetSpeedPacket.h>

NetworkUpdatePositionsAndVelocities::NetworkUpdatePositionsAndVelocities(GameManager* const gameManager, INetworkModule* const networkModule) :
        NetworkSystem<Transform, Kinematics>(gameManager, networkModule)
{}

void NetworkUpdatePositionsAndVelocities::update(const sf::Time& dt) {
    timer += dt.asSeconds();
    if(timer < 0.02) {
        return;
    }
    timer -= 0.02;
    if(auto serverModule = dynamic_cast<ServerNetworkModule*>(gameManager->getNetworkModule())) {
        auto& clients = serverModule->getServer().getClients();

        for(const auto& entity : entities) {
            auto transform = entity->getComponent<Transform>();
            auto kinematics = entity->getComponent<Kinematics>();
            for(const auto& client : clients) {
                auto transformPacket = serverModule->getNetwork()->create<SetTransformPacket>(entity->getID(), transform->position.x, transform->position.y, transform->rotation);
                auto velocityPacket = serverModule->getNetwork()->create<SetSpeedPacket>(entity->getID(), kinematics->velocity.x, kinematics->velocity.y);
                client->send(std::move(transformPacket));
                client->send(std::move(velocityPacket));
            }
        }
    } else {
        throw std::runtime_error("Cannot use NetworkUpdatePositionsAndVelocities on a client.");
    }
}