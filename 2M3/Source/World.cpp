#include <World.h>
#include <Car.h>

World::World()
{
	Car* player = new Car(10, sf::Vector2f(800, 450), sf::RectangleShape(sf::Vector2f(40, 20)));
	mEntities.push_back(player);
}

void World::update(sf::Time dt)
{
	for (auto& ent : mEntities)
	{
		ent->update(dt);
	}
}

void World::draw(sf::RenderTarget& target)
{
	for (auto& ent : mEntities)
	{
		ent->draw(target);
	}
}