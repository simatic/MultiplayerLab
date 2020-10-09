#pragma once
#include <vector>
#include <Entity.h>
#include <SFML/System.hpp>
#include "ResourceIdentifiers.h"
#include <PlayerGUI.h>
#include <queue>
#include <NetworkCommon.h>

class World
{
public:
							World(sf::RenderTarget& outputTarget, KeyBinding* keys1, KeyBinding* keys2, const FontHolder& fonts, bool local);
	void					initialize(EntityStruct p1, EntityStruct p2);

	void					update(sf::Time dt);
	void					clientUpdate(sf::Time dt);
	void					draw();
	void					loadTextures();
	bool					handleEvent(const sf::Event& event);

	sf::Vector2f			getWorldSize();
	Entity*					getEntityFromId(sf::Uint64 id);
	std::vector<Player*>&	getPlayers();
	Entity*					getUnassignedEntity();

	void					addCollision(Entity* ent1, Entity* ent2);
	void					createProjectile(sf::Uint64 id, sf::Vector2f pos, sf::Vector2f velocity, Car* creator, bool guided);
	void					createCar(sf::Uint64 id, sf::Vector2f pos, sf::Vector2f velocity, sf::Vector2f direction);

private:
	std::vector<Entity*>		mEntities;
	std::vector<Player*>		mPlayers;

	std::vector<Entity*>		mNewEntities;
	std::set<Entity::Pair>		mPairs;

	std::queue<Entity*>			mToBeAssignedID;

	sf::RenderTarget&			mTarget;
	TextureHolder				mTextures;

	KeyBinding*					mPlayerOneKeys;
	KeyBinding*					mPlayerTwoKeys;

	GUI::PlayerGUI				mPlayerOneGUI;
	GUI::PlayerGUI				mPlayerTwoGUI;

	float						mWorldWidth;
	float						mWorldHeight;

    void                        addWalls();
};