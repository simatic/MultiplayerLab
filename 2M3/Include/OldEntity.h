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

class Collider;

class OldEntity
{
public:
	typedef std::pair<OldEntity*, OldEntity*> Pair;

public:
	enum class Type
	{
		CarType,
		ProjectileType,
		PickUpType,
		Count
	};

public:
	OldEntity(sf::Vector2f pos, sf::RectangleShape colliderShape);

	virtual void		update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs);
	virtual void		draw(sf::RenderTarget& target);
	virtual bool		handleEvent(const sf::Event& event);
	virtual void		cleanUp(sf::Vector2f worldSize, sf::Time dt);

	Rectangle			getRectangle();
	Type				getType();
	sf::Uint64			getID();

	void				setID(sf::Uint64 id);

	void				setToRemove(bool toRemove);
	bool				toRemove();

	template <typename T>
	void addComponent(T& component);

	template <typename T>
	T* getComponent();

	template <typename T>
	void removeComponent();

protected:
	Type				mType;

	bool				mToRemove;

	sf::Uint64			mID;

private:
	std::unordered_map<std::size_t, std::unique_ptr<Component>> components;
};

template <typename T>
void OldEntity::addComponent(T& component)
{
	components[T::id] = std::make_unique<T>(component);
}

template <typename T>
T* OldEntity::getComponent()
{
	return static_cast<T*>(components[T::id].get());
}

template <typename T>
void OldEntity::removeComponent()
{
	components.erase(T::id);
}