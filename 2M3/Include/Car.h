#pragma once
#define _USE_MATH_DEFINES
#include <Entity.h>
#include <math.h>
#include <KeyBinding.h>
#include <Particles.h>

class Car : public Entity
{
public:
	enum class CarAction
	{
		ShootBullet,
		LaunchMissile,
		ActionCount
	};

public:
	Car();
	Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, KeyBinding* keys);

	void update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs) override;
	void getInput(sf::Time dt, std::vector<Entity*>& newEntities);
	void crash(sf::Vector2f otherVelocity);
	bool handleEvent(const sf::Event& event) override;
	bool needsEventInput();
	void cleanUp(sf::Vector2f worldSize, sf::Time dt) override;

	void draw(sf::RenderTarget& target) override;

	void damage(int points);
	void repair(int points);
	void addMissileAmmo(int ammo);

	void onCollision(Entity* other) override;

	std::string getActionText();
	float getSpeedRatio();

private:
	int mHP;
	int mHpMax;
	sf::RectangleShape mHpBackgroundBar;
	sf::RectangleShape mHpBar;

	KeyBinding* mKeyBindings;

	sf::Vector2f mCarDirection;

	const float mCarMaxSpeed = 1000;
	const float mCarBackwardsMaxSpeed = mCarMaxSpeed / 3;
	const float mCarAcceleration = 200;
	const float mTurnRadius = 12;
	const float mDriftTheshold = mCarMaxSpeed - 200;
	const float mDriftAngle = M_PI / 3;
	const float mDrag = 0.001f;

	bool mForward;
	bool mDrifting;
	float mPrevDriftingSign;

	bool mCrash;
	std::vector<Entity*> mPrevCollidedWith;
	std::vector<Entity*> mCollidedWith;

	sf::VertexArray mTires;
	Particles mDust;

	CarAction mAction;
	sf::Time mShootDelay;
	sf::Time mCurrentShootDelay;
	bool mLaunchedMissile;
	int mMissileAmmo;

};