#include <OldEntity.h>
#include "Common/Systems/RenderSystem.h"
#include "Common/Systems/CollisionSystem.h"
#include "Common/Systems/MovementSystem.h"
#include "ResourceHolder.h"

OldEntity::OldEntity(sf::Vector2f pos, sf::RectangleShape rect) :
	mTransform(pos, 0.0f),
	mVelocity(0.0f, 0.0f),
	mCollider(rect),
	mType(Type::Count),
	mToRemove(false),
	mID(0)
{
	sf::FloatRect bounds = mCollider.shape.getLocalBounds();
	mCollider.shape.setOrigin(bounds.width / 2, bounds.height / 2);

}

void OldEntity::update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs)
{
	CollisionSystem::update(dt, this, entities, pairs);
	MovementSystem::update(dt, this);

	mCollider.shape.setPosition(mTransform.position);
	mCollider.shape.setRotation(mTransform.rotation);
}

void OldEntity::serverUpdate(sf::Time serverTime, sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs)
{
	update(dt, entities, newEntities, pairs);
}

void OldEntity::draw(sf::RenderTarget& target)
{
	//target.draw(mCollider.shape);
	RenderSystem::render(this, target, mTransform);
}

sf::Vector2f& OldEntity::getPosition()
{
	return mTransform.position;
}

float OldEntity::getRotation()
{
	return mTransform.rotation;
}

sf::Vector2f OldEntity::getMiniMapPosition(sf::Vector2f worldSize, sf::Vector2f mapSize)
{
	float x = mapSize.x * mTransform.position.x / worldSize.x;
	float y = mapSize.y *  mTransform.position.y / worldSize.y;
	return sf::Vector2f(x, y);
}

sf::Vector2f OldEntity::getVelocity()
{
	return mVelocity;
}

Collider& OldEntity::getCollider()
{
	return mCollider;
}

OldEntity::Type OldEntity::getType()
{
	return mType;
}

void OldEntity::offset(sf::Vector2f o)
{
	mTransform.position += o;
}

void OldEntity::setVelocity(sf::Vector2f v)
{
	mVelocity = v;
}

bool OldEntity::toRemove()
{
	return mToRemove;
}

void OldEntity::remove()
{
	mToRemove = true;
}

void OldEntity::unremove()
{
	mToRemove = false;
}

Rectangle OldEntity::getRectangle()
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

bool OldEntity::handleEvent(const sf::Event& event)
{
	return true;
}

void OldEntity::cleanUp(sf::Vector2f worldSize, sf::Time dt)
{
}

sf::Uint64 OldEntity::getID()
{
	return mID;
}

void OldEntity::setID(sf::Uint64 id)
{
	mID = id;
}

void OldEntity::setPosition(sf::Vector2f p)
{
	mTransform.position = p;
}
