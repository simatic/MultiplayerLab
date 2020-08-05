#include <Entity.h>
#include <iostream>

Entity::Entity(sf::Vector2f pos, sf::RectangleShape rect) :
	mPosition(pos),
	mShape(rect)
{
	sf::FloatRect bounds = mShape.getLocalBounds();
	mShape.setOrigin(bounds.width / 2, bounds.height / 2);

	bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2, bounds.height / 2);
}

void Entity::update(sf::Time dt)
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