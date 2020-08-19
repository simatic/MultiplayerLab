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