#include "Common/Systems/GunSystem.h"

void GunSystem::update(const sf::Time dt, CarLogic* entity, std::vector<OldEntity*>& newEntities)
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
            entity->instanciateBullet(entity->getPosition(), gun->pointingDirection, newEntities);
            gun->elapsedTimeSinceLastShot = sf::Time::Zero;
        }
    }
    else
    {
        gun->elapsedTimeSinceLastShot += dt;
    }
}