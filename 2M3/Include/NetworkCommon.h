#pragma once
#include <string>
#include <SFML/Network.hpp>
#include <Entity.h>

// Where the server listens by default
//const std::string_view DEFAULT_HOST("localhost");
//const unsigned short DEFAULT_PORT(4096);

// Type of messages

// Packets originated in the server
enum class ServerMsgType
{
	ClientIdResponse,
	CarUpdate,
	Collision,
	ObjectCreation,
	ObjectDestruction,
	PingRequest,
	GameEnd
};

// Packets originated in the client
enum class ClientMsgType
{
	ClientIdRequest,
	Input,
	PingResponse
};


//structure for sending entity into packets, for entity creation or car updates.
//will still require some switch cases to take into account car actions and projectiles being guided or not
struct EntityStruct
{
	sf::Uint64 id;
	Entity::Type entityType;
	sf::Vector2f position;
	sf::Vector2f velocity;
};

sf::Packet& operator <<(sf::Packet& packet, const EntityStruct& entity);
sf::Packet& operator >>(sf::Packet& packet, EntityStruct& entity);

sf::Packet& operator <<(sf::Packet& packet, const sf::Time& time);
sf::Packet& operator >>(sf::Packet& packet, sf::Time& time);