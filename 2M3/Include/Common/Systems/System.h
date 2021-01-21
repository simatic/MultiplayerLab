#pragma once
#include "Common/Managers/Signature.h"
#include "Common/Entity.h"

#include <SFML/System/Time.hpp>
#include <set>

class GameManager;  // Forward redirection of GameManager.
class ClientNetworkModule;

enum class SystemType {
	Logic,
	Render,
	Network
};

/**
 * System class.
 */
template <SystemType type>
class System {
public:
	System(GameManager* const gameManager);
	virtual ~System() = default;

	virtual void start() {};
	virtual void update(const sf::Time& dt) {};

	void addEntity(Entity* entity);
	void removeEntity(Entity* entity);

	const Signature& getSignature() const;

protected:
	Signature signature;
	GameManager* const gameManager = nullptr;
	std::set<Entity*> entities;
};

template <SystemType type>
System<type>::System(GameManager* const gameManager) :
	gameManager(gameManager) 
{}

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

/**
 * SignedSystem class.
 */
template <SystemType type, typename... Components>
class SignedSystem : public System<type>
{
public:
	SignedSystem(GameManager* const gameManager);
};

template <SystemType type, typename... Components>
SignedSystem<type, Components...>::SignedSystem(GameManager* const gameManager) :
	System<type>(gameManager) {
	this->signature = Signature::generate<Components...>();
}

template <typename... Components>
using LogicSystem = SignedSystem<SystemType::Logic, Components...>;

template <typename... Components>
using RenderSystem = SignedSystem<SystemType::Render, Components...>;

template <typename... Components>
using NetworkSystem = SignedSystem<SystemType::Network, Components...>;
