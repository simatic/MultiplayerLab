#pragma once
#include "OldEntity.h"
#include <ProjectileLogic.h>
#include <math.h>
#include <KeyBinding.h>
#include <Particles.h>
#include <queue>
#include <functional>
#include "Common/Components/CarEngine.h"

struct Inputs
{
	bool up;
	bool down;
	bool left;
	bool right;
	bool action;
	bool changeActionEvent;
	bool doActionEvent;
};

struct SpatialVars
{
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f direction;
};

class CarLogic : public OldEntity
{
public:
	CarLogic();
	CarLogic(int hp, sf::Vector2f pos, sf::RectangleShape rect);
	CarLogic(int hp, sf::Vector2f pos, sf::RectangleShape rect, KeyBinding* keys);

	virtual void update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs) override;
	virtual void serverUpdate(sf::Time serverTime, sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs) override;
	void getInput(sf::Time serverTime);
	void crash(sf::Vector2f otherVelocity);
	bool handleEvent(const sf::Event& event) override;
	void cleanUp(sf::Vector2f worldSize, sf::Time dt) override;

	void damage(int points);
	void repair(int points);
	void addMissileAmmo(int ammo);

	void onCollision(OldEntity* other) override;

	sf::Vector2f getCarDirection();
	std::string getActionText();
	float getSpeedRatio();
	Inputs getSavedInputs();
	void setInputs(Inputs inputs);
	void insertInputs(sf::Time serverTime, Inputs inputs);

	void setCarDirection(sf::Vector2f d);

	void computeDeadReckoning(sf::Vector2f newPosition, sf::Vector2f newVelocity, sf::Vector2f newCarDirection);
	void stepUpDeadReckoning();

protected:
	virtual void instanciateBullet(const sf::Vector2f& position, const sf::Vector2f& direction, std::vector<OldEntity*>& newEntities);

protected:
	Inputs mInputs;

	int mMissileAmmo;

	std::function<ProjectileLogic* (sf::Vector2f position, sf::Vector2f direction)> mInstanciateMissile;
	std::function<ProjectileLogic* (sf::Vector2f position, sf::Vector2f direction)> mInstanciateBullet;

private:
	KeyBinding* mKeyBindings;

	std::map<sf::Time, Inputs> mServerInputs;

	bool mForward;
	bool mDrifting;
	float mPrevDriftingSign;

	bool mCrash;
	std::vector<OldEntity*> mPrevCollidedWith;
	std::vector<OldEntity*> mCollidedWith;

	sf::Time mShootDelay;
	sf::Time mCurrentShootDelay;
	bool mLaunchedMissile;

	std::queue<SpatialVars> mTrajectory;
};