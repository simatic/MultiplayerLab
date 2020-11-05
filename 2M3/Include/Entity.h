#pragma once
#include <SFML/Graphics.hpp>
#include <set>
#include <memory>
#include <Collider.h>
#include <Utility.h>
#include <unordered_map>
#include "ResourceIdentifiers.h"
#include "Common/Components/Component.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Collider.h"
#include "Common/Components/Component.h"

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

	sf::Vector2f&		getPosition();
	float				getRotation();
	sf::Vector2f		getMiniMapPosition(sf::Vector2f worldSize, sf::Vector2f mapSize);
	sf::Vector2f		getVelocity();
	Collider&			getCollider();
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

	virtual void		onCollision(Entity* other) = 0;

	template <typename T>
	void addComponent(T& component);

	template <typename T>
	T* getComponent();

	template <typename T>
	void removeComponent();

protected:
	Transform			mTransform;
	sf::Vector2f		mVelocity;

	sf::Sprite			mSprite;
	Collider			mCollider;

	Type				mType;

	bool				mToRemove;

	sf::Uint64			mID;

private:
	std::unordered_map<std::size_t, std::unique_ptr<Component>> components;
};

template <typename T>
void Entity::addComponent(T& component)
{
	components[T::id] = std::make_unique<T>(component);
}

template <typename T>
T* Entity::getComponent()
{
	return static_cast<T*>(components[T::id].get());
}

template <typename T>
void Entity::removeComponent()
{
	components.erase(T::id);
}