#pragma once
#include <vector>
#include <Entity.h>
#include <SFML/System.hpp>
#include <Player.h>

class World
{
public:
	World					(sf::RenderTarget& outputTarget, KeyBinding* keys1, KeyBinding* keys2);
	void					update(sf::Time dt);
	void					draw();
	bool					handleEvent(const sf::Event& event);

private:
	std::vector<Entity*>		mEntities;
	std::vector<Player*>		mPlayers;

	std::vector<Entity*>		mNewEntities;

	sf::RenderTarget&			mTarget;
};