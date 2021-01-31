#pragma once
#include "Common/Managers/Signature.h"
#include "Common/Entity.h"

#include <SFML/System/Time.hpp>
#include <set>

class GameManager;			// Forward redirection of GameManager.
class ClientNetworkModule;	// Forward redirection of ClientNetworkModule.
class ServerNetworkModule;	// Forward redirection of ServerNetworkModule.

/**
 * @enum SystemType
 * Defines different system types for different usages.
 */
enum class SystemType {
	Logic,
	Render,
	ServerNetwork,
	ClientNetwork
};

/**
 * @class Base System class.
 */
template <SystemType type>
class System {
public:
	System(GameManager* const gameManager);
	virtual ~System() = default;

	/**
	 * Start method is called once at the beginning of the game.
	 */
	virtual void start() {};

	/**
	 * Update method is called every frame.
	 * @param dt Time since the last frame.
	 */
	virtual void update(const sf::Time& dt) {};

	/**
	 * Adds an entity to the system's entities list.
	 * @param entity The entity.
	 */
	void addEntity(Entity* entity);

	/**
	 * Removes an entity from the system's entities list.
	 * @param entity The entity.
	 */
	void removeEntity(Entity* entity);

	/**
	 * Returns the signature of the system, which tells which components the system'entities need.
	 */
	const Signature& getSignature() const;

protected:
	Signature signature;						//!< Signature of the system.
	GameManager* const gameManager = nullptr;	//!< Pointer to the gameManager.
	std::set<Entity*> entities;					//!< System's entities list.
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
 * @class SignedSystem class.
 * Helper class to generate a compile-time signature for a given system,
 * depending on the components it requires to function properly.
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
 * @class LogicSystem
 * System that handles the logic of the game, independently from the network, such as physics, movement, particles...
 */
template <typename... Components>
using LogicSystem = SignedSystem<SystemType::Logic, Components...>;

/**
 * @class RenderSystem
 * System that handles rendering, sprites drawing and anything related to visual effects.
 */
template <typename... Components>
using RenderSystem = SignedSystem<SystemType::Render, Components...>;

/**
 * @class ClientNetworkSystem
 * System that communicates with the network on the client side.
 */
template <typename... Components>
class ClientNetworkSystem : public SignedSystem<SystemType::ClientNetwork, Components...> {
public:
	ClientNetworkSystem(GameManager* const gameManager, ClientNetworkModule* const networkModule);

protected:
	ClientNetworkModule* const networkModule;
};

template <typename... Components>
ClientNetworkSystem<Components...>::ClientNetworkSystem(GameManager* const gameManager, ClientNetworkModule* const networkModule) :
	SignedSystem<SystemType::ClientNetwork, Components...>(gameManager),
	networkModule(networkModule)
{}

/**
 * @class ServerNetworkSystem
 * System that communicates with the network on the server side.
 */
template <typename... Components>
class ServerNetworkSystem : public SignedSystem<SystemType::ServerNetwork, Components...> {
public:
	ServerNetworkSystem(GameManager* const gameManager, ServerNetworkModule* const networkModule);

protected:
	ServerNetworkModule* const networkModule;
};

template <typename... Components>
ServerNetworkSystem<Components...>::ServerNetworkSystem(GameManager* const gameManager, ServerNetworkModule* const networkModule) :
	SignedSystem<SystemType::ServerNetwork, Components...>(gameManager),
	networkModule(networkModule)
{}

