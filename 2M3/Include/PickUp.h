#pragma once
#include <OldEntity.h>

class PickUp : public OldEntity
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

	void onCollision(OldEntity* other);

	PickUpType getPickUpType();

private:
	PickUpType mType;

};