#include <PickUp.h>
#include <Car.h>

PickUp::PickUp(const TextureHolder& textures) :
	OldEntity(sf::Vector2f(0, 0), sf::RectangleShape(sf::Vector2f(0, 0))),
	mType(PickUpType::HealthPack)
{
}

PickUp::PickUp(PickUpType type, sf::Vector2f pos, const TextureHolder& textures) :
	OldEntity(pos, sf::RectangleShape(sf::Vector2f(15, 15))),
	mType(type)
{
}

PickUp::PickUpType PickUp::getPickUpType()
{
	return mType;
}