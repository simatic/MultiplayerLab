#pragma once
#include <SFML/Graphics.hpp>
#include <set>;
#include <Collider.h>
#include <Utility.h>

class Entity
{
public:
	typedef std::pair<Entity*, Entity*> Pair;

public:
	enum Type
	{
		CarType,
		ProjectileType,
		Count
	};

public:
	Entity(sf::Vector2f pos, sf::RectangleShape rect);

	virtual void		update(sf::Time dt, std::vector<Entity*>& newEntities);
	virtual void		draw(sf::RenderTarget& target);

	sf::Vector2f		getPosition();
	sf::Vector2f		getVelocity();
	sf::RectangleShape	getShape();
	Rectangle			getRectangle();
	Type				getType();
	bool				toRemove();
	void				remove();

	void				checkCollisions(std::vector<Entity*>& entities, std::set<Pair>& pairs);
	bool				collide(Entity* other);
	virtual void		onCollision(Entity* other) = 0;

protected:
	sf::Vector2f		mPosition;
	sf::Vector2f		mVelocity;
	float				mRotation;

	sf::Sprite			mSprite;
	sf::RectangleShape	mShape;

	Type				mType;

	bool				mToRemove;
};