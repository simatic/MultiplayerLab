#include "Common/Systems/CollisionSystem.h"

void CollisionSystem::update(const sf::Time& dt, OldEntity* entity, std::vector<OldEntity*>& others, std::set<OldEntity::Pair>& pairs)
{
	for (auto other : others)
	{
		if (other != entity)
		{
			bool collides = false;
			if (length(entity->getPosition() - other->getPosition()) > 100)
			{
				collides = false;
			}
			else if (entity->getCollider().shape.getGlobalBounds().intersects(other->getCollider().shape.getGlobalBounds()))
			{
				CollisionResult res = collision(entity->getRectangle(), other->getRectangle(), entity->getVelocity(), other->getVelocity(), dt);

				collides = res.intersect || res.willIntersect;
			}
			else
			{
				collides = false;
			}
			
			if (collides)
			{
				entity->getCollider().collides = true;
				entity->getCollider().others.emplace(other);
				pairs.insert(std::minmax(entity, other));
			}
		}
	}
}
