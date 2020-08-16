#pragma once
#include <Entity.h>

class PickUp : public Entity
{
public:
	enum class PickUpType
	{
		HealthPack,
		MissilesAmmo,
		Count
	};

public:
	PickUp();
	PickUp(PickUpType type, sf::Vector2f pos);

	void onCollision(Entity* other);

	PickUpType getPickUpType();

private:
	PickUpType mType;

};