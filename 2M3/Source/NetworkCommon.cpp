#include <NetworkCommon.h>

sf::Packet& operator<<(sf::Packet& packet, const EntityStruct& entity)
{
	return packet << entity.id << static_cast<sf::Uint32>(entity.entityType) << entity.position.x << entity.position.y << entity.velocity.x << entity.velocity.y;
}

sf::Packet& operator>>(sf::Packet& packet, EntityStruct& entity)
{
	packet >> entity.id; 
	
	sf::Uint32 type;
	packet >> type;
	entity.entityType = static_cast<Entity::Type>(entity.entityType);

	packet >> entity.position.x >> entity.position.y >> entity.velocity.x >> entity.velocity.y;
	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const sf::Time& time)
{
	return packet << time.asMicroseconds();
}

sf::Packet& operator>>(sf::Packet& packet, sf::Time& time)
{
	sf::Int64 t;
	packet >> t;
	time = sf::microseconds(t);

	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const sf::Keyboard::Key& key)
{
	return packet << static_cast<sf::Uint32>(key);
}

sf::Packet& operator>>(sf::Packet& packet, sf::Keyboard::Key& key)
{
	sf::Uint32 keyInt;
	packet >> keyInt;
	key = static_cast<sf::Keyboard::Key>(keyInt);
	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const Inputs& inputs)
{
	return packet << inputs.up << inputs.down << inputs.left << inputs.right << inputs.action << inputs.changeActionEvent << inputs.doActionEvent;
}

sf::Packet& operator>>(sf::Packet& packet, Inputs& inputs)
{
	return packet >> inputs.up >> inputs.down >> inputs.left >> inputs.right >> inputs.action >> inputs.changeActionEvent >> inputs.doActionEvent;
}

sf::Packet& operator<<(sf::Packet& packet, const ServerMsgType& msgType)
{
	return packet << static_cast<sf::Uint32>(msgType);
}

sf::Packet& operator>>(sf::Packet& packet, ServerMsgType& msgType)
{
	sf::Uint32 msg;
	packet >> msg;
	msgType = static_cast<ServerMsgType>(msg);
	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const ClientMsgType& msgType)
{
	return packet << static_cast<sf::Uint32>(msgType);
}

sf::Packet& operator>>(sf::Packet& packet, ClientMsgType& msgType)
{
	sf::Uint32 msg;
	packet >> msg;
	msgType = static_cast<ClientMsgType>(msg);
	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const sf::Vector2f& vector)
{
	return packet << vector.x << vector.y;
}

sf::Packet& operator>>(sf::Packet& packet, sf::Vector2f& vector)
{
	packet >> vector.x >> vector.y;
	return packet;
}