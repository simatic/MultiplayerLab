#include "Common/Systems/GunSystem.h"
#include "Common/Managers/GameManager.h"

#include "Prefabs/Prefabs.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/Bullet.h"

#include "Utility.h"

GunSystem::GunSystem(GameManager* const gameManager) :
    LogicSystem<CarInput, CarEngine, Gun>(gameManager)
{}

void GunSystem::update(const sf::Time& dt)
{
    for (Entity* entity: entities)
    {
        CarInput* inputs = entity->getComponent<CarInput>();
        CarEngine* engine = entity->getComponent<CarEngine>();
        Gun* gun = entity->getComponent<Gun>();

        gun->pointingDirection = engine->direction;
        if (engine->isDrifting) gun->pointingDirection = rotate(gun->pointingDirection, engine->previousDriftingSign * engine->driftAngle);

        if (gun->elapsedTimeSinceLastShot >= gun->cooldown)
        {
            if (inputs->action)
            {
                std::cout << "Shoot from " << entity << std::endl;
                std::shared_ptr<Entity> bullet = Prefab::createBullet(true);
                bullet->getComponent<Transform>()->position = entity->getComponent<Transform>()->position;
                bullet->getComponent<Bullet>()->owner = entity->weak_from_this();
                bullet->getComponent<Kinematics>()->velocity = bullet->getComponent<Bullet>()->maxSpeed * gun->pointingDirection;
                gameManager->addEntity(bullet);
                gun->elapsedTimeSinceLastShot = sf::Time::Zero;
            }
        }
        else
        {
            gun->elapsedTimeSinceLastShot += dt;
        }
    }
}