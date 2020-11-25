#include <CarLogic.h>
#include <Utility.h>
#include <iostream>
#include <ProjectileLogic.h>
#include "ResourceHolder.h"
#include "NetworkCommon.h"
#include "Common/Components/Health.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/CarInput.h"
#include "Common/Systems/KeyboardInputSystem.h"
#include <math.h>


CarLogic::CarLogic() :
	CarLogic(1, sf::Vector2f(0, 0), sf::RectangleShape(sf::Vector2f(0, 0)))
{}

CarLogic::CarLogic(int hp, sf::Vector2f pos, sf::RectangleShape rect) :
	CarLogic(hp, pos, rect, nullptr)
{}

CarLogic::CarLogic(int hp, sf::Vector2f pos, sf::RectangleShape rect, KeyBinding* keys) :
	mKeyBindings(keys),
	mShootDelay(sf::seconds(0.1)),
	mCrash(false),
	mLaunchedMissile(false),
	mMissileAmmo(5),
	mInputs({ false, false, false, false, false, false, false }),
	mTrajectory(),
	OldEntity(pos, rect)
{
	CarInput inputs = CarInput();
	addComponent<CarInput>(inputs);

	Health health = Health(hp, hp);
	addComponent<Health>(health);

	CarEngine engine = CarEngine(1000, 1000 / 3, 200, 24, 800, M_PI / 3, 0.001f, sf::Vector2f(1, 0));
	addComponent<CarEngine>(engine);

	mType = Type::CarType;

	mInstanciateMissile = [this](sf::Vector2f position, sf::Vector2f direction) -> ProjectileLogic* 
	{ 
		return new ProjectileLogic(5, sf::seconds(10), 400, 400, position + 25.f * direction, direction, sf::RectangleShape(sf::Vector2f(30, 10)), this); 
	};

	mInstanciateBullet = [this](sf::Vector2f position, sf::Vector2f direction) -> ProjectileLogic*
	{
		return new ProjectileLogic(1, sf::seconds(1), 1500, position + 25.f * direction, direction, sf::RectangleShape(sf::Vector2f(5, 5)), this);
	};
}

void CarLogic::update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs)
{
	if (mCurrentShootDelay > sf::Time::Zero) mCurrentShootDelay -= dt;

	if (!mTrajectory.empty()) //maybe skip usual computation when we use dead reckoning?
	{
		stepUpDeadReckoning();
	}
	else
	{
		getInput();
		useInputs(dt, newEntities);
	}

	OldEntity::update(dt, entities, newEntities, pairs);
}

void CarLogic::serverUpdate(sf::Time serverTime, sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs)
{
	if (mCurrentShootDelay > sf::Time::Zero) mCurrentShootDelay -= dt;

	getInput(serverTime);
	useInputs(dt, newEntities);

	OldEntity::update(dt, entities, newEntities, pairs);

	mInputs = { false, false, false, false, false, false, false };
}

void CarLogic::getInput()
{
	KeyboardInputSystem::update(this, mKeyBindings);
}

void CarLogic::getInput(sf::Time serverTime)
{
	std::map<sf::Time, Inputs>::iterator low;
	low = mServerInputs.lower_bound(serverTime);
	if (low == mServerInputs.begin())
	{
		//serverTime is lower than every input timestamp
		mInputs = { false, false, false, false, false, false, false };
	}
	else
	{
		--low;
		mInputs = low->second;
	}
}

void CarLogic::useInputs(sf::Time dt, std::vector<OldEntity*>& newEntities)
{
	Kinematics* kinematics = getComponent<Kinematics>();
	CarInput* inputs = getComponent<CarInput>();
	CarEngine* engine = getComponent<CarEngine>();
	
	float l = length(kinematics->velocity);

	float angle = 0;
	float angleSign = 0;
	if (inputs->left && l > 50)
	{
		angle += M_PI / 3;
		angleSign += 1;
	}
	if (inputs->right && l > 50)
	{
		angle -= M_PI / 3;
		angleSign -= 1;
	}

	float accel = 0;
	bool driftBrake = false;
	if (inputs->up)
	{
		float f = 1;
		if (!engine->isGoingForward) f = 10;
		accel += f * engine->acceleration;
	}
	if (inputs->down)
	{
		if (engine->isGoingForward && l > engine->driftThreshold && angleSign != 0) driftBrake = true;
		else
		{
			float f = 1;
			if (engine->isGoingForward) f = 10;
			accel -= f * engine->acceleration;
		}
	}
	if (accel == 0 && l > 200)
	{
		accel = (l * l + 2 * l) * engine->drag;
		if (engine->isGoingForward) accel *= -1;
	}
	else if (accel == 0)
	{
		kinematics->velocity = sf::Vector2f(0, 0);
	}

	float tangAccel = accel * cos(angle);
	float radAccel = accel * sin(angle);
	sf::Vector2f tangAccelVector = tangAccel * engine->direction;
	kinematics->velocity += tangAccelVector * dt.asSeconds();
	l = length(kinematics->velocity);
	if (engine->isGoingForward && l > engine->maxSpeed)
	{
		kinematics->velocity *= engine->maxSpeed / l;
	}
	else if (!engine->isGoingForward && l > engine->backwardsMaxSpeed)
	{
		kinematics->velocity *= engine->backwardsMaxSpeed / l;
	}

	bool prevDrifting = engine->isDrifting;
	engine->isDrifting = engine->isGoingForward && l > engine->driftThreshold && angleSign != 0 && driftBrake;

	float theta = sqrt(abs(radAccel) / engine->turnRadius) * dt.asSeconds();
	engine->isGoingForward = dotProduct(kinematics->velocity, engine->direction) >= 0;
	if (!engine->isGoingForward)
	{
		kinematics->velocity = rotate(kinematics->velocity, -theta * angleSign);
		engine->direction = rotate(engine->direction, -theta * angleSign);
	}
	else
	{
		kinematics->velocity = rotate(kinematics->velocity, theta * angleSign);
		engine->direction = rotate(engine->direction, theta * angleSign);
	}

	if (prevDrifting && !engine->isDrifting)
	{
		engine->direction = rotate(engine->direction, engine->previousDriftingSign * engine->driftThreshold);
		kinematics->velocity = rotate(kinematics->velocity, engine->previousDriftingSign * engine->driftAngle);
	}
	engine->previousDriftingSign = angleSign;

	float carAngle = 0;
	if (engine->direction.x != 0) carAngle = -atan2(engine->direction.y, engine->direction.x);
	if (engine->direction.x == 0 && engine->direction.y != 0) carAngle = M_PI_2 * engine->direction.y / abs(engine->direction.y);
	if (engine->isDrifting) carAngle += angleSign * engine->driftAngle;
	getRotation() = -carAngle * 180.0 / M_PI;

	sf::Vector2f projDir = engine->direction;
	if (engine->isDrifting) projDir = rotate(projDir, angleSign * engine->driftAngle);

	if (inputs->action) //&& mCurrentShootDelay <= sf::Time::Zero)
	{
		if (mCurrentShootDelay <= sf::Time::Zero)
		{
			mCurrentShootDelay = mShootDelay;

			instanciateBullet(getPosition(), projDir, newEntities);
		}
	}
}

bool CarLogic::handleEvent(const sf::Event& event)
{
	CarInput* inputs = getComponent<CarInput>();
	if (mKeyBindings != nullptr)
	{
		inputs->changeActionEvent = inputs->changeActionEvent || event.type == sf::Event::KeyPressed && event.key.code == mKeyBindings->getAssignedKey(PlayerAction::ChangeAction);
		inputs->doActionEvent = inputs->doActionEvent || event.type == sf::Event::KeyPressed && event.key.code == mKeyBindings->getAssignedKey(PlayerAction::DoAction);
	}

	return true;
}

void CarLogic::cleanUp(sf::Vector2f worldSize, sf::Time dt)
{
	if (getPosition().x > worldSize.x || getPosition().x < 0) getPosition().x -= getVelocity().x * dt.asSeconds();
	if (getPosition().y > worldSize.y || getPosition().y < 0) getPosition().y -= getVelocity().y * dt.asSeconds();

	mPrevCollidedWith = mCollidedWith;
	mCollidedWith.clear();
}

void CarLogic::crash(sf::Vector2f otherVelocity)
{
	mCrash = true;
	//mVelocity = sf::Vector2f(0, 0);
	setVelocity(otherVelocity);
}

void CarLogic::damage(int points)
{
	Health* h = getComponent<Health>();
	h->health -= points;
	std::cout << "took " << points << " dmg" << std::endl;
	if (h->health <= 0) mToRemove = true;
}

void CarLogic::repair(int points)
{
	Health* h = getComponent<Health>();
	h->health += points;
	if (h->health > h->maxHealth) h->health = h->maxHealth;
}

void CarLogic::addMissileAmmo(int ammo)
{
	mMissileAmmo += ammo;
}

void CarLogic::onCollision(OldEntity* other)
{
	switch (other->getType())
	{
	case Type::CarType:
	{
		Car* otherCar = dynamic_cast<Car*>(other);
		mCollidedWith.push_back(other);

		bool collision = true;
		for (auto& ent : mPrevCollidedWith)
		{
			if (ent == other)
			{
				collision = false;
				break;
			}
		}

		if (collision)
		{
			std::cout << "collision" << std::endl;
			damage(2);
			otherCar->damage(2);
		}

		sf::Vector2f prevVelocity = getVelocity();
		//crash(other->getVelocity());
		//otherCar->crash(prevVelocity);

		/*setVelocity(0.8f * otherCar->getVelocity());
		otherCar->setVelocity(0.8f * prevVelocity);*/
		setVelocity(sf::Vector2f(0, 0));
		other->setVelocity(sf::Vector2f(0, 0));

		break;
	}

	case Type::ProjectileType:
	{
		ProjectileLogic* otherProj = dynamic_cast<ProjectileLogic*>(other);
		if (otherProj->getCar() == this)
		{
			break;
		}

		damage(otherProj->getDamage());
		other->remove();
		break;
	}
	default:
		break;
	}
}

sf::Vector2f CarLogic::getCarDirection()
{
	return getComponent<CarEngine>()->direction;
}

float CarLogic::getSpeedRatio()
{
	return length(getVelocity()) / getComponent<CarEngine>()->maxSpeed;
}

Inputs CarLogic::getSavedInputs()
{
	return mInputs;
}

void CarLogic::setInputs(Inputs inputs)
{
	mInputs = inputs;
}

void CarLogic::computeDeadReckoning(sf::Vector2f newPosition, sf::Vector2f newVelocity, sf::Vector2f newCarDirection)
{
	int numberOfSteps;
	numberOfSteps = floor(TimePerFrame / TimePerTick);
	mTrajectory.empty();

	for (int i = 0; i < numberOfSteps; i++)
	{
		mTrajectory.push({ getPosition() + ((1.f / (numberOfSteps - i)) * (newPosition - getPosition())), getVelocity() + ((1.f / (numberOfSteps - i)) * (newVelocity - getVelocity())), getComponent<CarEngine>()->direction + ((1.f / (numberOfSteps - i)) * (newCarDirection - getComponent<CarEngine>()->direction)) });
	}
}

void CarLogic::stepUpDeadReckoning()
{
	getPosition() = mTrajectory.front().position;
	setVelocity(mTrajectory.front().velocity);
	getComponent<CarEngine>()->direction = mTrajectory.front().direction;
	mTrajectory.pop();
}

void CarLogic::insertInputs(sf::Time serverTime, Inputs inputs)
{
	mServerInputs.emplace(serverTime, inputs);
}

void CarLogic::setCarDirection(sf::Vector2f d)
{
	getComponent<CarEngine>()->direction = d;
}

void CarLogic::instanciateBullet(const sf::Vector2f& position, const sf::Vector2f& direction, std::vector<OldEntity*>& newEntities)
{
	std::cout << "CarLogic" << std::endl;
	ProjectileLogic* proj = new ProjectileLogic(1, sf::seconds(1), 1500, position + 25.f * direction, direction, sf::RectangleShape(sf::Vector2f(5, 5)), this);
	newEntities.push_back(proj); 
}