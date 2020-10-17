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
		PickUpType,
		Count
	};

public:
	Entity(sf::Vector2f pos, sf::RectangleShape colliderShape);

	virtual void		update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs);
	virtual void		serverUpdate(sf::Time serverTime, sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs);
	virtual void		draw(sf::RenderTarget& target);
	virtual bool		handleEvent(const sf::Event& event);
	virtual void		cleanUp(sf::Vector2f worldSize, sf::Time dt);

	sf::Vector2f		getPosition();
	float				getRotation();
	sf::Vector2f		getMiniMapPosition(sf::Vector2f worldSize, sf::Vector2f mapSize);
	sf::Vector2f		getVelocity();
	sf::RectangleShape	getShape();
	Rectangle			getRectangle();
	Type				getType();
	sf::Uint64			getID();

	void				setID(sf::Uint64 id);
	void				setPosition(sf::Vector2f p);
	void				offset(sf::Vector2f o);
	void				setVelocity(sf::Vector2f v);

	bool				toRemove();
	void				remove();
	void				unremove();

	void				checkCollisions(std::vector<Entity*>& entities, std::set<Pair>& pairs, sf::Time dt);
	bool				collide(Entity* other, sf::Time dt);
	virtual void		onCollision(Entity* other) = 0;

protected:
	sf::Vector2f		mPosition;
	sf::Vector2f		mVelocity;
	float				mRotation;

	sf::Sprite			mSprite;
	sf::RectangleShape	mColliderShape;

	Type				mType;

	bool				mToRemove;

	sf::Uint64			mID;
};