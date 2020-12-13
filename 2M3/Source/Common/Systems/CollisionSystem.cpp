#include "Common/Systems/CollisionSystem.h"
#include "Common/Components/Kinematics.h"
#include "Utility.h"
#include "Collider.h"

Rectangle getRectangle(const Collider& collider);

CollisionSystem::CollisionSystem(GameManager* const gameManager) :
	LogicSystem<Transform, Collider>(gameManager)
{}

void CollisionSystem::update(const sf::Time& dt)
{
    for (Entity* entity: entities) {
        auto collider = entity->getComponent<Collider>();
        auto transform = entity->getComponent<Transform>();
        collider->shape.setOrigin(collider->shape.getSize().x/2, collider->shape.getSize().y/2);
        collider->shape.setPosition(transform->position);
        collider->shape.setRotation(transform->rotation);
    }

	for (Entity* entity: entities)
	{
		Collider* collider = entity->getComponent<Collider>();
        collider->others.clear();

        for (Entity* other: entities)
		{
			if (other != entity)
			{
			    auto otherCollider = other->getComponent<Collider>();
			    float minDistancePossible = std::max(
			            std::max(collider->shape.getSize().x, collider->shape.getSize().y),
			            std::max(otherCollider->shape.getSize().x, otherCollider->shape.getSize().y)
                );

				bool collides = false;
				if (length(entity->getComponent<Transform>()->position - other->getComponent<Transform>()->position) > minDistancePossible)
				{
                    continue;
				}
				else if (collider->shape.getGlobalBounds().intersects(otherCollider->shape.getGlobalBounds()))
				{
				    sf::Vector2f velocityA{};
				    sf::Vector2f velocityB{};

				    if(auto kinematics = entity->getComponent<Kinematics>()) {
				        velocityA = kinematics->velocity;
				    }
				    if(auto kinematics = other->getComponent<Kinematics>()) {
				        velocityB = kinematics->velocity;
				    }
					CollisionResult res = collision(getRectangle(*collider), getRectangle(*otherCollider), velocityA, velocityB, dt);

					collides = res.intersect || res.willIntersect;
				}

				if (collides)
				{
					collider->collides = true;
					collider->others.emplace_back(other->weak_from_this());
				}
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