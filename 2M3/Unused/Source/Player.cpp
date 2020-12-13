#include <Player.h>
#include <iostream>

Player::Player(int i, KeyBinding* keys, const TextureHolder& textures) :
	mPlayerID(i)
{

	/*mPlayerView = sf::View(sf::FloatRect(0, 0, 1600, 900));
	mPlayerView.setViewport(sf::FloatRect(0, 0, 1, 1));*/
	if (mPlayerID == 0)
	{
		mPlayerView = sf::View(sf::FloatRect(0, 0, 800, 900));
		mPlayerView.setViewport(sf::FloatRect(0, 0, 0.5, 1));
	}
	else if (mPlayerID == 1)
	{
		mPlayerView = sf::View(sf::FloatRect(800, 0, 800, 900));
		mPlayerView.setViewport(sf::FloatRect(0.5, 0, 0.5, 1));

	}
	mPlayerCar = new Car(100, sf::Vector2f(800, 450), sf::RectangleShape(sf::Vector2f(80, 40)), keys, textures);
	mPlayerView.setCenter(mPlayerCar->getComponent<Transform>()->position);
}

// TODO check if this ctor is really needed or not.
Player::Player(int i, Car* car) :
	mPlayerID(i),
	mPlayerCar(car)
{
	if (mPlayerID == 0)
	{
		mPlayerView = sf::View(sf::FloatRect(0, 0, 800, 900));
		mPlayerView.setViewport(sf::FloatRect(0, 0, 0.5, 1));
	}
	else if (mPlayerID == 1)
	{
		mPlayerView = sf::View(sf::FloatRect(800, 0, 800, 900));
		mPlayerView.setViewport(sf::FloatRect(0.5, 0, 0.5, 1));

	}
	mPlayerView.setCenter(car->getComponent<Transform>()->position);
}

void Player::update(sf::Time dt) //, std::vector<Entity*>& newEntities)
{
	//mPlayerView.move(mPlayerCar->getVelocity() * dt.asSeconds());
	mPlayerView.setCenter(mPlayerCar->getComponent<Transform>()->position);
}

void Player::draw(sf::RenderTarget& target, std::vector<OldEntity*>& entities)
{
	//target.setView(mPlayerView);
	// Test area
	sf::View view(target.getDefaultView());
	view.setCenter(mPlayerCar->getPosition());
	target.setView(view);
	// End of test area
	for (auto& ent : entities)
	{
		ent->draw(target);
	}
}

Car* Player::getCar()
{
	return mPlayerCar;
}

int Player::getID()
{
	return mPlayerID;
}
