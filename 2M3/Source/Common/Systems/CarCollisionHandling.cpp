#include "Common/Systems/CarCollisionHandling.h"
#include "Common/Components/Bullet.h"
#include "GameManager.h"

void CarCollisionSystem::update(const sf::Time&)
{
	for (Entity* entity: entities)
	{
		Collider* collider = entity->getComponent<Collider>();

		if (collider->collides)
		{
			for (std::weak_ptr<Entity> wkOther: collider->others)
			{
				if (std::shared_ptr<Entity> other = wkOther.lock())
				{
					switch (other->getLayer())
					{
					case Layer::ProjectileLayer:
					{
						Health* health = entity->getComponent<Health>();
						Bullet* bullet = other->getComponent<Bullet>();

						if (bullet->owner.lock().get() != entity)
						{
							health->health -= bullet->damage;
							GameManager::getInstance()->removeEntityNextFrame(other->getID());
						}
						break;
					}

					case Layer::CarLayer:
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
				
			}

			collider->others.clear();
		}
	}
    
}