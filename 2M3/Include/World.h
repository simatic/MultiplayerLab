#pragma once
#include <vector>
#include <Entity.h>
#include <SFML/System.hpp>
#include "ResourceIdentifiers.h"
#include <PlayerGUI.h>

class World
{
public:
							World(sf::RenderTarget& outputTarget, KeyBinding* keys1, KeyBinding* keys2, const FontHolder& fonts);
	void					update(sf::Time dt);
	void					draw();
	void					loadTextures();
	bool					handleEvent(const sf::Event& event);

	sf::Vector2f			getWorldSize();
	Entity*					getEntityFromId(sf::Uint64 id);

	void					addCollision(Entity* ent1, Entity* ent2);
	void					createEntity(Entity* ent);

private:
	std::vector<Entity*>		mEntities;
	std::vector<Player*>		mPlayers;

	std::vector<Entity*>		mNewEntities;
	std::set<Entity::Pair>		mPairs;

	sf::RenderTarget&			mTarget;
	TextureHolder				mTextures;

	GUI::PlayerGUI				mPlayerOneGUI;
	GUI::PlayerGUI				mPlayerTwoGUI;

	float						mWorldWidth;
	float						mWorldHeight;
};