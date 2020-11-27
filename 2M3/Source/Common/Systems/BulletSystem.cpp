#include "Common/Systems/BulletSystem.h"
#include "Utility.h"
#include "CarLogic.h"

void BulletSystem::update(const sf::Time& dt, OldEntity* entity, std::vector<OldEntity*>& others, std::set<OldEntity::Pair>& pairs)
{
	Kinematics* kinematics = entity->getComponent<Kinematics>();
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
			sf::Vector2f acceleration = unitVector(bullet->target->getComponent<Transform>()->position - transform->position);
			sf::Vector2f velocity = unitVector(unitVector(kinematics->velocity) + bullet->guideRate * acceleration) * bullet->maxSpeed;
			kinematics->velocity = velocity;
		}
		else
		{
			float minDist = bullet->detectionRange;
			OldEntity* target = nullptr;
			for (const auto ent : others)
			{
				float dist = length(transform->position - ent->getComponent<Transform>()->position);
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
	if (kinematics->velocity.x != 0) angle = -atan2(kinematics->velocity.y, kinematics->velocity.x);
	if (kinematics->velocity.x == 0 && kinematics->velocity.y != 0) angle = M_PI_2 * kinematics->velocity.y / abs(kinematics->velocity.y);
	transform->rotation = -toDegrees(angle);
}