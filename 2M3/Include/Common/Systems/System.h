#pragma once
#include "Common/Managers/Signature.h"
#include "Common/Entity.h"

#include <SFML/System/Time.hpp>
#include <set>

enum class SystemType {
	Logic,
	Render
};

template <SystemType type>
class System {
public:
	System() = default;
	virtual ~System() = default;

	virtual void start() {};
	virtual void update(const sf::Time& dt) {};

	void addEntity(Entity* entity);
	void removeEntity(Entity* entity);

	const Signature& getSignature() const;

protected:
	Signature signature;
	std::set<Entity*> entities;
};

template <SystemType type>
void System<type>::addEntity(Entity* entity) {
	entities.emplace(entity);
}

template <SystemType type>
void System<type>::removeEntity(Entity* entity) {
	entities.erase(entity);
}

template <SystemType type>
const Signature& System<type>::getSignature() const {
	return signature;
}

template <SystemType type, typename... Components>
class SignedSystem : public System<type>
{
public:
	SignedSystem();
};

template <SystemType type, typename... Components>
SignedSystem<type, Components...>::SignedSystem() :
	System<type>()
{
	signature = Signature::generate<Components...>();
}

template <typename... Components>
class LogicSystem : public SignedSystem<SystemType::Logic, Components...> 
{};

template <typename... Components>
class RenderSystem : public SignedSystem<SystemType::Render, Components...>
{};