#include <OldEntity.h>
#include "Common/Components/Kinematics.h"
#include "Common/Systems/RenderSystem.h"
#include "Common/Systems/CollisionSystem.h"
#include "Common/Systems/MovementSystem.h"
#include "ResourceHolder.h"

OldEntity::OldEntity(sf::Vector2f pos, sf::RectangleShape rect) :
	mType(Type::Count),
	mToRemove(false),
	mID(0)
{
	Transform t = Transform(pos, 0.0f);
	addComponent<Transform>(t);

	Kinematics k = Kinematics();
	addComponent<Kinematics>(k);

	Collider c = Collider(rect);
	sf::FloatRect bounds = c.shape.getLocalBounds();
	c.shape.setOrigin(bounds.width / 2, bounds.height / 2);
	addComponent<Collider>(c);
}

void OldEntity::update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs)
{
	CollisionSystem::update(dt, this, entities, pairs);
	MovementSystem::update(dt, this);

	Transform* t = getComponent<Transform>();
	Collider* c = getComponent<Collider>();

	c->shape.setPosition(t->position);
	c->shape.setRotation(t->rotation);
}

void OldEntity::draw(sf::RenderTarget& target)
{
	//target.draw(getCollider().shape);
	RenderSystem::render(this, target, *getComponent<Transform>());
}

sf::Vector2f OldEntity::getMiniMapPosition(sf::Vector2f worldSize, sf::Vector2f mapSize)
{
	float x = mapSize.x * getComponent<Transform>()->position.x / worldSize.x;
	float y = mapSize.y *  getComponent<Transform>()->position.y / worldSize.y;
	return sf::Vector2f(x, y);
}

Collider& OldEntity::getCollider()
{
	return *getComponent<Collider>();
}

OldEntity::Type OldEntity::getType()
{
	return mType;
}

void OldEntity::offset(sf::Vector2f o)
{
	getComponent<Transform>()->position += o;
}

void OldEntity::setVelocity(sf::Vector2f v)
{
	getComponent<Kinematics>()->velocity = v;
}

void OldEntity::setToRemove(bool toRemove)
{
	mToRemove = toRemove;
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
	sf::FloatRect baseRect = getCollider().shape.getLocalBounds();
	std::vector<sf::Vector2f> points;
	float halfW = baseRect.width / 2.f;
	float halfH = baseRect.height / 2.f;
	points.push_back(sf::Vector2f(baseRect.left - halfW, baseRect.top - halfH));
	points.push_back(sf::Vector2f(baseRect.left + halfW, baseRect.top - halfH));
	points.push_back(sf::Vector2f(baseRect.left - halfW, baseRect.top + halfH));
	points.push_back(sf::Vector2f(baseRect.left + halfW, baseRect.top + halfH));

	for (int i = 0; i < 4; i++)
	{
		points[i].x *= getCollider().shape.getScale().x;
		points[i].y *= getCollider().shape.getScale().y;
		points[i] = rotate(points[i], -toRadians(getCollider().shape.getRotation()));
		points[i] += getCollider().shape.getPosition();
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
	getComponent<Transform>()->position = p;
}
