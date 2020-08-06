#include "..\Include\Entity.h"
#include <Entity.h>
#include <iostream>

Entity::Entity(sf::Vector2f pos, sf::RectangleShape rect) :
	mPosition(pos),
	mShape(rect),
	mToRemove(false)
{
	sf::FloatRect bounds = mShape.getLocalBounds();
	mShape.setOrigin(bounds.width / 2, bounds.height / 2);

	bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2, bounds.height / 2);
}

void Entity::update(sf::Time dt, std::vector<Entity*>& newEntities)
{
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

bool Entity::toRemove()
{
	return mToRemove;
}

void Entity::remove()
{
	mToRemove = true;
}

bool Entity::collide(Entity* other)
{
	return mShape.getGlobalBounds().intersects(other->mShape.getGlobalBounds());
}

void Entity::checkCollisions(std::vector<Entity*>& entities, std::set<Pair>& pairs)
{
	for (auto ent : entities)
	{
		if (ent != this && collide(ent))
		{
			pairs.insert(std::minmax(this, ent));
		}
	}
}