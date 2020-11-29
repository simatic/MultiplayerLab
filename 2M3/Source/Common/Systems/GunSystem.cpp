#include "Common/Systems/GunSystem.h"
#include "Utility.h"

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
                std::cout << "TODO Instanciate bullet" << std::endl;
                //entity->instanciateBullet(entity->getComponent<Transform>()->position, gun->pointingDirection, newEntities);
                gun->elapsedTimeSinceLastShot = sf::Time::Zero;
            }
        }
        else
        {
            gun->elapsedTimeSinceLastShot += dt;
        }
    }
}