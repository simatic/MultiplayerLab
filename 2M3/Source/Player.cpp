#include <Player.h>
#include <iostream>

Player::Player(int i, KeyBinding* keys1, KeyBinding* keys2) :
	mPlayerID(i)
{
	if (i == 0)
	{
		mPlayerView = sf::View(sf::FloatRect(0, 0, 800, 900));
		mPlayerView.setViewport(sf::FloatRect(0, 0, 0.5, 1));
		mPlayerCar = new Car(10, sf::Vector2f(800, 450), sf::RectangleShape(sf::Vector2f(40, 20)), keys1);
	}
	else if (i == 1)
	{
		mPlayerView = sf::View(sf::FloatRect(800, 0, 800, 900));
		mPlayerView.setViewport(sf::FloatRect(0.5, 0, 0.5, 1));
		mPlayerCar = new Car(10, sf::Vector2f(850, 450), sf::RectangleShape(sf::Vector2f(40, 20)), keys2);
	}
	mPlayerView.setCenter(mPlayerCar->getPosition());
}

void Player::update(sf::Time dt, std::vector<Entity*>& newEntities)
{
	mPlayerView.move(mPlayerCar->getVelocity() * dt.asSeconds());
}

void Player::draw(sf::RenderTarget& target, std::vector<Entity*>& entities)
{
	target.setView(mPlayerView);
	for (auto& ent : entities)
	{
		ent->draw(target);
	}
}

Car* Player::getCar()
{
	return mPlayerCar;
}