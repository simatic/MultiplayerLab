#pragma once
#include <Entity.h>
#include <ProjectileLogic.h>
#include <math.h>
#include <KeyBinding.h>
#include <Particles.h>
#include <queue>

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

class CarLogic : public Entity
{
public:
	enum class CarAction
	{
		ShootBullet,
		LaunchMissile,
		ToggleMap,
		ActionCount
	};

public:
	CarLogic();
	CarLogic(int hp, sf::Vector2f pos, sf::RectangleShape rect);
	CarLogic(int hp, sf::Vector2f pos, sf::RectangleShape rect, KeyBinding* keys);

	virtual void update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs) override;
	virtual void serverUpdate(sf::Time serverTime, sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs) override;
	void getInput();
	void getInput(sf::Time serverTime);
	virtual void useInputs(sf::Time dt, std::vector<Entity*>& newEntities);
	void crash(sf::Vector2f otherVelocity);
	bool handleEvent(const sf::Event& event) override;
	bool needsEventInput();
	void cleanUp(sf::Vector2f worldSize, sf::Time dt) override;

	void damage(int points);
	void repair(int points);
	void addMissileAmmo(int ammo);

	void onCollision(Entity* other) override;

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
	//virtual ProjectileLogic* instanciateProjectile(int dmg, sf::Time lifetime, float speed, sf::Vector2f pos, sf::Vector2f direction, sf::RectangleShape rect);

protected:
	Inputs mInputs;

	int mHP;
	int mHpMax;

	sf::Vector2f mCarDirection;

	CarAction mAction;

	int mMissileAmmo;

private:
	KeyBinding* mKeyBindings;

	std::map<sf::Time, Inputs> mServerInputs;

	const float mCarMaxSpeed = 1000;
	const float mCarBackwardsMaxSpeed = mCarMaxSpeed / 3;
	const float mCarAcceleration = 200;
	const float mTurnRadius = 24;
	const float mDriftTheshold = mCarMaxSpeed - 200;
	const float mDriftAngle = M_PI / 3;
	const float mDrag = 0.001f;

	bool mForward;
	bool mDrifting;
	float mPrevDriftingSign;

	bool mCrash;
	std::vector<Entity*> mPrevCollidedWith;
	std::vector<Entity*> mCollidedWith;

	sf::Time mShootDelay;
	sf::Time mCurrentShootDelay;
	bool mLaunchedMissile;

	std::queue<SpatialVars> mTrajectory;
};
