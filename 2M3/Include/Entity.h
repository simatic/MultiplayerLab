#pragma once
#include <SFML/Graphics.hpp>

class Entity
{
public:
	Entity(sf::Vector2f pos, sf::RectangleShape rect);

	virtual void		update(sf::Time);
	void				draw();

	void				setVelocity(float vx, float vy);
	void				setVelocity(sf::Vector2f v);
	void				addVelocity(float vx, float vy);
	void				addVelocity(sf::Vector2f v);
	sf::Vector2f		getVelocity() const;

private:
	sf::Vector2f		mPosition;
	sf::Vector2f		mVelocity;
	float				mRotation;

	sf::Sprite			mSprite;
	sf::RectangleShape	mShape;
};