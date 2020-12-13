#pragma once
#include <vector>
#include "OldEntity.h"
#include "Common/Components/Component.h"
#include <SFML/System.hpp>
#include "ResourceIdentifiers.h"
#include <PlayerGUI.h>
#include <queue>

class World
{
public:
<<<<<<< HEAD:2M3/Include/World.h
							World(int uid, sf::RenderTexture& outputTarget, KeyBinding* keys1, const FontHolder& fonts, bool local);
	void					initialize(EntityStruct p1, EntityStruct p2);
=======
							World(sf::RenderTarget& outputTarget, KeyBinding* keys1, const FontHolder& fonts, bool local);
>>>>>>> Modular_Architecture:2M3/Unused/Include/World.h

	void					update(sf::Time dt);
	void					draw();
	void					loadTextures();
	bool					handleEvent(const sf::Event& event);

	sf::Vector2f			getWorldSize();
	OldEntity*					getEntityFromId(sf::Uint64 id);
	std::vector<Player*>&	getPlayers();
	OldEntity*					getUnassignedEntity();

	void					addCollision(OldEntity* ent1, OldEntity* ent2);
	void					createProjectile(sf::Uint64 id, sf::Vector2f pos, sf::Vector2f velocity, Car* creator, bool guided);
	void					createCar(sf::Uint64 id, sf::Vector2f pos, sf::Vector2f velocity, sf::Vector2f direction);

private:
	std::vector<OldEntity*>		mEntities;
	std::vector<Player*>		mPlayers;

	std::vector<OldEntity*>		mNewEntities;
	std::set<OldEntity::Pair>		mPairs;

	std::queue<OldEntity*>			mToBeAssignedID;

	sf::RenderTexture&			mTarget;
	TextureHolder				mTextures;

	KeyBinding*					mPlayerOneKeys;

	GUI::PlayerGUI				mPlayerOneGUI;
	GUI::PlayerGUI				mPlayerTwoGUI;

	float						mWorldWidth;
	float						mWorldHeight;

    void                        addWalls();
};