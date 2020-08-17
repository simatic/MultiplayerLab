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
	PickUp(const TextureHolder& textures);
	PickUp(PickUpType type, sf::Vector2f pos, const TextureHolder& textures);

	void onCollision(Entity* other);

	PickUpType getPickUpType();

private:
	PickUpType mType;

};