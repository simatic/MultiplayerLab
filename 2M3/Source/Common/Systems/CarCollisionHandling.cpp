#include "Common/Systems/CarCollisionHandling.h"
#include "Common/Components/Bullet.h"
#include "GameManager.h"

CarCollisionSystem::CarCollisionSystem(GameManager* const gameManager) :
	LogicSystem<Kinematics, Collider, Health>(gameManager)
{}

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
							std::cout << "Collision " << entity << " - Owner " << bullet->owner.lock().get() << std::endl;
							health->health -= bullet->damage;
							gameManager->removeEntityNextFrame(other.get());
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

                    case Layer::WallLayer:
                    {
                        Kinematics* kinematics = entity->getComponent<Kinematics>();
                        // TODO: use speed reduction of collision function
                        kinematics->velocity = sf::Vector2f(0, 0);
                        break;
                    }
					
					default:
						break;
					}
				}
				
			}
		}
	}
    
}