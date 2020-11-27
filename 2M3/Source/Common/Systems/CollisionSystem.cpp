#include "Common/Systems/CollisionSystem.h"
#include "Common/Components/Kinematics.h"

void CollisionSystem::update(const sf::Time& dt, OldEntity* entity, std::vector<OldEntity*>& others, std::set<OldEntity::Pair>& pairs)
{
	Collider* collider = entity->getComponent<Collider>();

	for (auto other : others)
	{
		if (other != entity)
		{
			bool collides = false;
			if (length(entity->getComponent<Transform>()->position - other->getComponent<Transform>()->position) > 100)
			{
				collides = false;
			}
			else if (collider->shape.getGlobalBounds().intersects(other->getComponent<Collider>()->shape.getGlobalBounds()))
			{
				CollisionResult res = collision(entity->getRectangle(), other->getRectangle(), entity->getComponent<Kinematics>()->velocity, other->getComponent<Kinematics>()->velocity, dt);

				collides = res.intersect || res.willIntersect;
			}
			else
			{
				collides = false;
			}
			
			if (collides)
			{
				collider->collides = true;
				collider->others.emplace(other);
				pairs.insert(std::minmax(entity, other));
			}
		}
	}
}
