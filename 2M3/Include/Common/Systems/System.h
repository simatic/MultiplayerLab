#pragma once
#include "Common/Managers/Signature.h"
#include "Common/Entity.h"

#include <SFML/System/Time.hpp>
#include <set>

class GameManager;  // Forward redirection of GameManager.
class INetworkModule;

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

/**
 * Logic system: System that handles the logic of the game, independently from the network, such as physics, movement, particles...
 */
template <typename... Components>
using LogicSystem = SignedSystem<SystemType::Logic, Components...>;

/**
 * Render system: System that handles rendering, sprites drawing and anything related to visual effects
 */
template <typename... Components>
using RenderSystem = SignedSystem<SystemType::Render, Components...>;

/**
 * Network system: System that communicates with the network
 */
template <typename... Components>
class NetworkSystem : public SignedSystem<SystemType::Network, Components...> {
public:
	NetworkSystem(GameManager* const gameManager, INetworkModule* const networkModule);

protected:
	INetworkModule* const networkModule;
};

template <typename... Components>
NetworkSystem<Components...>::NetworkSystem(GameManager* const gameManager, INetworkModule* const networkModule) :
	SignedSystem<SystemType::Network, Components...>(gameManager),
	networkModule(networkModule)
{}
