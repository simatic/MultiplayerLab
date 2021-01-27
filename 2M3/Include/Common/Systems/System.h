#pragma once
#include "Common/Managers/Signature.h"
#include "Common/Entity.h"

#include <SFML/System/Time.hpp>
#include <set>

class GameManager;  // Forward redirection of GameManager.
class ClientNetworkModule;
class ServerNetworkModule;

enum class SystemType {
	Logic,
	Render,
	ServerNetwork,
	ClientNetwork
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
 * Network client system: System that communicates with the network on the client side
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
 * Network server system: System that communicates with the network on the server side
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

