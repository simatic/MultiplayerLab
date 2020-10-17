#pragma once
#include <SFML/Graphics.hpp>
#include <Car.h>
#include <vector>
#include <KeyBinding.h>
#include "ResourceIdentifiers.h"

// A voir en rapport à sa nouvelle place (au niveau de thread lorsqu'il est initialisé dans Application.
class Player
{
public:
	Player(int i, KeyBinding* keys, const TextureHolder& textures);
	Player(int i, Car* car);

	void update(sf::Time dt); //, std::vector<Entity*>& newEntities);
	void draw(sf::RenderTarget& target, std::vector<Entity*>& entities);

	Car* getCar();
	int getID();

private:
	sf::View	mPlayerView;
	Car*		mPlayerCar;
	int			mPlayerID;
};