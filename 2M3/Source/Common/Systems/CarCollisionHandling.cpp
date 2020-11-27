#include "Common/Systems/CarCollisionHandling.h"
#include "Common/Components/Bullet.h"

void CarCollisionHandling::update(OldEntity* entity)
{
    Collider* collider = entity->getComponent<Collider>();

	if (collider->collides)
	{
		for (OldEntity* other: collider->others)
		{
			switch (other->getType())
			{
			case OldEntity::Type::ProjectileType:
			{
				Health* health = entity->getComponent<Health>();
            	Bullet* bullet = other->getComponent<Bullet>();

				if (bullet->owner != entity)
                {
                    health->health -= bullet->damage;
                    other->setToRemove(true);
                }
				break;
			}

			case OldEntity::Type::CarType:
			{
				Health* health = entity->getComponent<Health>();
				Kinematics* kinematics = entity->getComponent<Kinematics>();

				health->health -= 2;
				kinematics->velocity = sf::Vector2f(0, 0);
				break;
			}
            
			default:
				break;
			}
		}

		collider->others.clear();
	}
}