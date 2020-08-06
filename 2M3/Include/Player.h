#pragma once
#include <SFML/Graphics.hpp>
#include <Car.h>
#include <vector>
#include <KeyBinding.h>

class Player
{
public:
	Player(int i);

	void update(sf::Time dt);
	void draw(sf::RenderTarget& target, std::vector<Entity*>& entities);

	Car* getCar();

private:
	sf::View	mPlayerView;
	Car*		mPlayerCar;
	int			mPlayerID;
};