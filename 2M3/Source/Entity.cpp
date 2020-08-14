#include <Entity.h>
#include <iostream>

Entity::Entity(sf::Vector2f pos, sf::RectangleShape rect) :
	mPosition(pos),
	mShape(rect),
	mRotation(0),
	mType(Type::Count),
	mToRemove(false)
{
	sf::FloatRect bounds = mShape.getLocalBounds();
	mShape.setOrigin(bounds.width / 2, bounds.height / 2);

	bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2, bounds.height / 2);
}

void Entity::update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs)
{
	checkCollisions(entities, pairs, dt);

	mPosition += mVelocity * dt.asSeconds();

	mShape.setPosition(mPosition);
	mShape.setRotation(mRotation);
	mSprite.setRotation(mRotation);
}

void Entity::draw(sf::RenderTarget& target)
{
	target.draw(mShape);
	//target.draw(mSprite);
}

sf::Vector2f Entity::getPosition()
{
	return mPosition;
}

sf::Vector2f Entity::getVelocity()
{
	return mVelocity;
}

sf::RectangleShape Entity::getShape()
{
	return mShape;
}

Entity::Type Entity::getType()
{
	return mType;
}

void Entity::offset(sf::Vector2f o)
{
	mPosition += o;
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

bool Entity::collide(Entity* other, sf::Time dt)
{
	if (length(mPosition - other->getPosition()) > 100) return false;
	//if (mShape.getGlobalBounds().intersects(other->getShape().getGlobalBounds())) return false;

	Rectangle rect = getRectangle();
	if (mShape.getGlobalBounds().intersects(other->mShape.getGlobalBounds()))
	{
		CollisionResult res = collision(rect, other->getRectangle(), mVelocity, other->getVelocity(), dt);

		/*mVelocity += res.speedReductionA;
		other->setVelocity(other->getVelocity() + res.speedReductionB);*/
		/*mPosition -= res.speedReductionA;
		other->offset(-res.speedReductionB);*/

		return res.intersect || res.willIntersect;
	}
	return false;
}

void Entity::checkCollisions(std::vector<Entity*>& entities, std::set<Pair>& pairs, sf::Time dt)
{
	for (auto ent : entities)
	{
		if (ent != this && collide(ent, dt))
		{
			pairs.insert(std::minmax(this, ent));
		}
	}
}

Rectangle Entity::getRectangle()
{
	sf::FloatRect baseRect = mShape.getLocalBounds();
	std::vector<sf::Vector2f> points;
	float halfW = baseRect.width / 2.f;
	float halfH = baseRect.height / 2.f;
	points.push_back(sf::Vector2f(baseRect.left - halfW, baseRect.top - halfH));
	points.push_back(sf::Vector2f(baseRect.left + halfW, baseRect.top - halfH));
	points.push_back(sf::Vector2f(baseRect.left - halfW, baseRect.top + halfH));
	points.push_back(sf::Vector2f(baseRect.left + halfW, baseRect.top + halfH));

	for (int i = 0; i < 4; i++)
	{
		points[i].x *= mShape.getScale().x;
		points[i].y *= mShape.getScale().y;
		points[i] = rotate(points[i], -toRadians(mShape.getRotation()));
		points[i] += mShape.getPosition();
	}

	Rectangle rect;
	rect.points = points;
	return rect;

}

bool Entity::handleEvent(const sf::Event& event)
{
	return true;
}