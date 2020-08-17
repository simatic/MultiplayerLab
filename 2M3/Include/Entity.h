#pragma once
#include <SFML/Graphics.hpp>
#include <set>
#include <Collider.h>
#include <Utility.h>
#include "ResourceIdentifiers.h"

class Entity
{
public:
	typedef std::pair<Entity*, Entity*> Pair;

public:
	enum class Type
	{
		CarType,
		ProjectileType,
		Count
	};

public:
	Entity(sf::Vector2f pos, sf::RectangleShape rect, const TextureHolder& textures);

	virtual void		update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs);
	virtual void		draw(sf::RenderTarget& target);

	sf::Vector2f		getPosition();
	sf::Vector2f		getVelocity();
	sf::RectangleShape	getShape();
	Rectangle			getRectangle();
	Type				getType();
	const TextureHolder&getTextures();
	float				getRotation();

	void				offset(sf::Vector2f o);
	void				setVelocity(sf::Vector2f v);
	void				setSprite();

	bool				toRemove();
	void				remove();

	void				checkCollisions(std::vector<Entity*>& entities, std::set<Pair>& pairs, sf::Time dt);
	bool				collide(Entity* other, sf::Time dt);
	virtual void		onCollision(Entity* other) = 0;

protected:
	sf::Vector2f		mPosition;
	sf::Vector2f		mVelocity;
	float				mRotation;

	sf::Sprite			mSprite;
	sf::RectangleShape	mShape;
	const TextureHolder&mTextures;

	Type				mType;

	bool				mToRemove;
};