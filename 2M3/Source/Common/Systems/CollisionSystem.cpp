#include "Common/Systems/CollisionSystem.h"

void CollisionSystem::update(const sf::Time& dt, Entity* entity, std::vector<Entity*>& others, std::set<Entity::Pair>& pairs)
{
	for (auto other : others)
	{
		if (other != entity)
		{
			if (length(entity->getPosition() - other->getPosition()) > 100)
			{
				entity->getCollider().collides = false;
			}
			else if (entity->getCollider().shape.getGlobalBounds().intersects(other->getCollider().shape.getGlobalBounds()))
			{
				CollisionResult res = collision(entity->getRectangle(), other->getRectangle(), entity->getVelocity(), other->getVelocity(), dt);

				entity->getCollider().collides = res.intersect || res.willIntersect;
			}
			else
			{
				entity->getCollider().collides = false;
			}
			
			if (entity->getCollider().collides)
			{
				pairs.insert(std::minmax(entity, other));
			}
		}
	}
}
