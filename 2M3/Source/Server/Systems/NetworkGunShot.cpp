#include <Server/Modules/ServerNetworkModule.h>
#include <Common/Network/AddEntityPacket.h>
#include <Common/Network/SetTransformPacket.h>
#include <Common/Network/SetSpeedPacket.h>
#include <Server/Systems/NetworkGunShot.h>
#include "Common/Managers/GameManager.h"

#include "Prefabs/Prefabs.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/Bullet.h"
#include "Server/Server.h"

#include "Utility.h"

NetworkGunShot::NetworkGunShot(GameManager* const gameManager, ServerNetworkModule* networkModule) :
    ServerNetworkSystem<CarInput, CarEngine, Gun>(gameManager, networkModule)
{}

void NetworkGunShot::update(const sf::Time& dt)
{
    for (Entity *entity: entities) {
        CarInput *inputs = entity->getComponent<CarInput>();
        CarEngine *engine = entity->getComponent<CarEngine>();
        Gun *gun = entity->getComponent<Gun>();

        gun->pointingDirection = engine->direction;
        if (engine->isDrifting)
            gun->pointingDirection = rotate(gun->pointingDirection,
                                            engine->previousDriftingSign * engine->driftAngle);

        if (gun->elapsedTimeSinceLastShot >= gun->cooldown) {
            if (inputs->action) {
                std::shared_ptr<Entity> bullet = Prefab::createBullet(false);
                bullet->getComponent<Transform>()->position = entity->getComponent<Transform>()->position;
                bullet->getComponent<Bullet>()->owner = entity->weak_from_this();
                bullet->getComponent<Kinematics>()->velocity =
                        bullet->getComponent<Bullet>()->maxSpeed * gun->pointingDirection;
                auto id = networkModule->getNewNetworkID();
                gameManager->addEntityWithIDNextFrame(bullet, id);
                for(auto& client : networkModule->getServer().getClients()) {
                    client->send(networkModule->create<AddEntityPacket>(Prefab::Type::Bullet, id));
                    client->send(networkModule->create<SetTransformPacket>(id, bullet->getComponent<Transform>()));
                    client->send(networkModule->create<SetVelocityPacket>(id, bullet->getComponent<Kinematics>()->velocity));
                }
                gun->elapsedTimeSinceLastShot = sf::Time::Zero;
            }
        } else {
            gun->elapsedTimeSinceLastShot += dt;
        }
    }
}