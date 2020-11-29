#pragma once
#include "Common/Managers/Signature.h"
#include "Common/Entity.h"

#include <SFML/System/Time.hpp>
#include <set>

class System
{
public:
	virtual void update(const sf::Time& dt) = 0;

	void addEntity(Entity* entity) { entities.emplace(entity); }
	void removeEntity(Entity* entity) { entities.erase(entity); }

	Signature signature;

protected:
	std::set<Entity*> entities;
};

template <typename... Components>
class SignedSystem : public System
{
public:
	SignedSystem();
};

template <typename... Components>
SignedSystem<Components...>::SignedSystem() :
	System()
{
	signature = Signature::generate<Components...>();
}