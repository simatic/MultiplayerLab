#include <World.h>
#include <Car.h>

World::World()
{
	//Car* player = new Car(10, sf::Vector2f(800, 450), sf::RectangleShape(sf::Vector2f(40, 20)));
	//mEntities.push_back(player);

	Player* p1 = new Player(0);
	Player* p2 = new Player(1);
	mPlayers.push_back(p1);
	mPlayers.push_back(p2);

	mEntities.push_back(p1->getCar());
	mEntities.push_back(p2->getCar());
}

void World::update(sf::Time dt)
{
	for (auto& ent : mEntities)
	{
		ent->update(dt);
	}
	for (auto& player : mPlayers)
	{
		player->update(dt);
	}
}

void World::draw(sf::RenderTarget& target)
{
	/*for (auto& ent : mEntities)
	{
		ent->draw(target);
	}*/
	for (auto& player : mPlayers)
	{
		player->draw(target, mEntities);
	}
}