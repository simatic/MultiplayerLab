#include "Common/Systems/BulletSystem.h"
#include "Utility.h"
#include "CarLogic.h"

void BulletSystem::update(const sf::Time& dt, OldEntity* entity, std::vector<OldEntity*>& others, std::set<OldEntity::Pair>& pairs)
{
    Transform* transform = entity->getComponent<Transform>();
    Bullet* bullet = entity->getComponent<Bullet>();

    bullet->lifetime -= dt;
	if (bullet->lifetime < sf::Time::Zero)
	{
		entity->setToRemove(true);
	}
	//mTransform.rotation = mCar->getRotation();

	if (bullet->target != nullptr && bullet->target->toRemove()) bullet->target = nullptr;

	if (bullet->guided)
	{
		if (bullet->target != nullptr)
		{
			sf::Vector2f acceleration = unitVector(bullet->target->getPosition() - transform->position);
			sf::Vector2f velocity = unitVector(unitVector(entity->getVelocity()) + bullet->guideRate * acceleration) * bullet->maxSpeed;
			entity->setVelocity(velocity);
		}
		else
		{
			float minDist = bullet->detectionRange;
			OldEntity* target = nullptr;
			for (const auto ent : others)
			{
				float dist = length(transform->position - ent->getPosition());
				if (dist < minDist && ent->getType() == OldEntity::Type::CarType && ent != static_cast<ProjectileLogic*>(entity)->getCar())
				{
					minDist = dist;
					target = ent;
				}
			}
			bullet->target = target;
		}
	}

	float angle = 0;
	if (entity->getVelocity().x != 0) angle = -atan2(entity->getVelocity().y, entity->getVelocity().x);
	if (entity->getVelocity().x == 0 && entity->getVelocity().y != 0) angle = M_PI_2 * entity->getVelocity().y / abs(entity->getVelocity().y);
	transform->rotation = -toDegrees(angle);
}