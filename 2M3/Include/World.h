#pragma once
#include <vector>
#include <Entity.h>
#include <SFML/System.hpp>
#include "ResourceIdentifiers.h"
#include <PlayerGUI.h>

class World
{
public:
	World					(sf::RenderTarget& outputTarget, KeyBinding* keys1, KeyBinding* keys2, const FontHolder& fonts);
	void					update(sf::Time dt);
	void					draw();
	void					loadTextures();
	bool					handleEvent(const sf::Event& event);

	sf::Vector2f			getWorldSize();

private:
	std::vector<Entity*>		mEntities;
	std::vector<Player*>		mPlayers;

	std::vector<Entity*>		mNewEntities;

	sf::RenderTarget&			mTarget;
	TextureHolder				mTextures;

	GUI::PlayerGUI				mPlayerOneGUI;
	GUI::PlayerGUI				mPlayerTwoGUI;

	float						mWorldWidth;
	float						mWorldHeight;
};