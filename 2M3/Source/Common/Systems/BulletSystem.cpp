#include "Common/Systems/BulletSystem.h"
#include "Common/Managers/GameManager.h"
#include "Utility.h"

BulletSystem::BulletSystem(GameManager* const gameManager) :
	LogicSystem<Transform, Kinematics, Bullet>(gameManager)
{}

void BulletSystem::update(const sf::Time& dt)
{
	for (Entity* entity: entities)
	{
		Kinematics* kinematics = entity->getComponent<Kinematics>();
		Transform* transform = entity->getComponent<Transform>();
		Bullet* bullet = entity->getComponent<Bullet>();

		bullet->lifetime -= dt;
		if (bullet->lifetime < sf::Time::Zero)
		{
			gameManager->removeEntityNextFrame(entity);
		}
		//mTransform.rotation = mCar->getRotation();

		float angle = 0;
		if (kinematics->velocity.x != 0) angle = -atan2(kinematics->velocity.y, kinematics->velocity.x);
		if (kinematics->velocity.x == 0 && kinematics->velocity.y != 0) angle = M_PI_2 * kinematics->velocity.y / abs(kinematics->velocity.y);
		transform->rotation = -toDegrees(angle);
	}
}