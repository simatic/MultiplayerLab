#include <Entity.h>
#include <iostream>
#include "Common/Systems/RenderSystem.h"
#include "Common/Systems/CollisionSystem.h"
#include "ResourceHolder.h"

Entity::Entity(sf::Vector2f pos, sf::RectangleShape rect) :
	mTransform(pos, 0.0f),
	mCollider(rect),
	mType(Type::Count),
	mToRemove(false),
	mID(0)
{
	sf::FloatRect bounds = mCollider.shape.getLocalBounds();
	mCollider.shape.setOrigin(bounds.width / 2, bounds.height / 2);

}

void Entity::update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs)
{
	CollisionSystem::update(dt, this, entities, pairs);

	mTransform.position += mVelocity * dt.asSeconds();

	mCollider.shape.setPosition(mTransform.position);
	mCollider.shape.setRotation(mTransform.rotation);
}

void Entity::serverUpdate(sf::Time serverTime, sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs)
{
	update(dt, entities, newEntities, pairs);
}

void Entity::draw(sf::RenderTarget& target)
{
	//target.draw(mCollider.shape);
	RenderSystem::render(target, mTransform, Sprite(mSprite));
}

sf::Vector2f Entity::getPosition()
{
	return mTransform.position;
}

float Entity::getRotation()
{
	return mTransform.rotation;
}

sf::Vector2f Entity::getMiniMapPosition(sf::Vector2f worldSize, sf::Vector2f mapSize)
{
	float x = mapSize.x * mTransform.position.x / worldSize.x;
	float y = mapSize.y *  mTransform.position.y / worldSize.y;
	return sf::Vector2f(x, y);
}

sf::Vector2f Entity::getVelocity()
{
	return mVelocity;
}

Collider& Entity::getCollider()
{
	return mCollider;
}

Entity::Type Entity::getType()
{
	return mType;
}

void Entity::offset(sf::Vector2f o)
{
	mTransform.position += o;
}

void Entity::setVelocity(sf::Vector2f v)
{
	mVelocity = v;
}

bool Entity::toRemove()
{
	return mToRemove;
}

void Entity::remove()
{
	mToRemove = true;
}

void Entity::unremove()
{
	mToRemove = false;
}

Rectangle Entity::getRectangle()
{
	sf::FloatRect baseRect = mCollider.shape.getLocalBounds();
	std::vector<sf::Vector2f> points;
	float halfW = baseRect.width / 2.f;
	float halfH = baseRect.height / 2.f;
	points.push_back(sf::Vector2f(baseRect.left - halfW, baseRect.top - halfH));
	points.push_back(sf::Vector2f(baseRect.left + halfW, baseRect.top - halfH));
	points.push_back(sf::Vector2f(baseRect.left - halfW, baseRect.top + halfH));
	points.push_back(sf::Vector2f(baseRect.left + halfW, baseRect.top + halfH));

	for (int i = 0; i < 4; i++)
	{
		points[i].x *= mCollider.shape.getScale().x;
		points[i].y *= mCollider.shape.getScale().y;
		points[i] = rotate(points[i], -toRadians(mCollider.shape.getRotation()));
		points[i] += mCollider.shape.getPosition();
	}

	Rectangle rect;
	rect.points = points;
	return rect;

}

bool Entity::handleEvent(const sf::Event& event)
{
	return true;
}

void Entity::cleanUp(sf::Vector2f worldSize, sf::Time dt)
{
}

sf::Uint64 Entity::getID()
{
	return mID;
}

void Entity::setID(sf::Uint64 id)
{
	mID = id;
}

void Entity::setPosition(sf::Vector2f p)
{
	mTransform.position = p;
}
