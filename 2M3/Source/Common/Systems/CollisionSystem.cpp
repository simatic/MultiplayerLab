#include "Common/Systems/CollisionSystem.h"
#include "Common/Components/Kinematics.h"

Rectangle getRectangle(const Collider& collider);

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
				CollisionResult res = collision(getRectangle(*collider), getRectangle(*other->getComponent<Collider>()), entity->getComponent<Kinematics>()->velocity, other->getComponent<Kinematics>()->velocity, dt);

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

Rectangle getRectangle(const Collider& collider)
{
	sf::FloatRect baseRect = collider.shape.getLocalBounds();
	std::vector<sf::Vector2f> points;
	float halfW = baseRect.width / 2.f;
	float halfH = baseRect.height / 2.f;
	points.push_back(sf::Vector2f(baseRect.left - halfW, baseRect.top - halfH));
	points.push_back(sf::Vector2f(baseRect.left + halfW, baseRect.top - halfH));
	points.push_back(sf::Vector2f(baseRect.left - halfW, baseRect.top + halfH));
	points.push_back(sf::Vector2f(baseRect.left + halfW, baseRect.top + halfH));

	for (int i = 0; i < 4; i++)
	{
		points[i].x *= collider.shape.getScale().x;
		points[i].y *= collider.shape.getScale().y;
		points[i] = rotate(points[i], -toRadians(collider.shape.getRotation()));
		points[i] += collider.shape.getPosition();
	}

	Rectangle rect;
	rect.points = points;
	return rect;
}