#pragma once
#include <SFML/Graphics.hpp>

class Entity
{
public:
	Entity(sf::Vector2f pos, sf::RectangleShape rect);

	virtual void		update(sf::Time dt, std::vector<Entity*>& newEntities);
	virtual void		draw(sf::RenderTarget& target);

	sf::Vector2f		getPosition();
	sf::Vector2f		getVelocity();

protected:
	sf::Vector2f		mPosition;
	sf::Vector2f		mVelocity;
	float				mRotation;

	sf::Sprite			mSprite;
	sf::RectangleShape	mShape;
};