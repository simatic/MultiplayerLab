#include <Car.h>
#include <Utility.h>
#include <iostream>
#include <Projectile.h>
#include <PickUp.h>
#include "ResourceHolder.h"
#include "NetworkCommon.h"
#include "Common/Components/Sprite.h"
#include "Common/Components/Trajectory.h"
#include "Common/Components/Particles.h"
#include "Common/Components/Health.h"
#include "Common/Systems/RenderSystem.h"
#include "Common/Systems/TrajectorySystem.h"
#include "Common/Systems/RenderTrajectorySystem.h"
#include "Common/Systems/ParticleSystem.h"
#include "Common/Systems/RenderParticleSystem.h"
#include <math.h>


Car::Car(const TextureHolder& textures) :
	CarLogic(),
	mShowMap(false),
	mTextures(textures)
{
	setSprite();

	Trajectory tr = Trajectory();
	tr.trajectory[0].position = getPosition();
	addComponent<Trajectory>(tr);

	Particles particles = Particles(sf::Vector2f(0, 0), sf::Color::White, sf::seconds(0.7), sf::seconds(1.0 / 40.0));
	addComponent<Particles>(particles);
}

Car::Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, const TextureHolder& textures) :
	Car(hp, pos, rect, nullptr, textures)
{}

Car::Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, KeyBinding* keys, const TextureHolder& textures) :
	CarLogic(hp, pos, rect, keys),
	mShowMap(false),
	mTextures(textures)
{
	setSprite();

	Trajectory tr = Trajectory();
	tr.trajectory[0].position = getPosition();
	addComponent<Trajectory>(tr);

	Particles particles = Particles(sf::Vector2f(0, 0), sf::Color::White, sf::seconds(0.7), sf::seconds(1.0 / 40.0));
	addComponent<Particles>(particles);

	mHpBackgroundBar = sf::RectangleShape(sf::Vector2f(40, 10));
	mHpBackgroundBar.setFillColor(sf::Color(127, 127, 127));
	mHpBackgroundBar.setOrigin(sf::Vector2f(20, 5));
	mHpBar = sf::RectangleShape(sf::Vector2f(40, 10));
	mHpBar.setFillColor(sf::Color::Green);

	mInstanciateMissile = [this](sf::Vector2f position, sf::Vector2f direction) -> ProjectileLogic*
	{
		return new Projectile(5, sf::seconds(10), 400, 400, position + 25.f * direction, direction, sf::RectangleShape(sf::Vector2f(30, 10)), this, mTextures);
	};

	mInstanciateBullet = [this](sf::Vector2f position, sf::Vector2f direction) -> ProjectileLogic*
	{
		return new Projectile(1, sf::seconds(1), 1500, position + 25.f * direction, direction, sf::RectangleShape(sf::Vector2f(5, 5)), this, mTextures);
	};
}

void Car::update(sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs)
{
	CarLogic::update(dt, entities, newEntities, pairs);
	TrajectorySystem::update(this);
	ParticleSystem::update(this, dt);
}

void Car::serverUpdate(sf::Time serverTime, sf::Time dt, std::vector<OldEntity*> entities, std::vector<OldEntity*>& newEntities, std::set<Pair>& pairs)
{
	CarLogic::serverUpdate(serverTime, dt, entities, newEntities, pairs);
	TrajectorySystem::update(this);
	ParticleSystem::update(this, dt);
}

void Car::useInputs(sf::Time dt, std::vector<OldEntity*>& newEntities)
{
	CarLogic::useInputs(dt, newEntities);

	if (mInputs.changeActionEvent)
	{
		mShowMap = false;
	}
	else if (mInputs.doActionEvent && needsEventInput() && mAction == CarLogic::CarAction::ToggleMap)
	{
		mShowMap = !mShowMap;
	}
}

void Car::draw(sf::RenderTarget& target)
{
	RenderTrajectorySystem::render(this, target);
	RenderParticleSystem::render(target, this);

	mHpBackgroundBar.setPosition(getPosition() + sf::Vector2f(0, 50));
	target.draw(mHpBackgroundBar);
	float hpWidth = mHpBackgroundBar.getSize().x;
	float hpHeight = mHpBackgroundBar.getSize().y;

	Health* h = getComponent<Health>();

	float hpBarWidth = hpWidth * h->health / (float)h->maxHealth;
	mHpBar.setPosition(mHpBackgroundBar.getPosition() - sf::Vector2f(hpWidth / 2.f, hpHeight / 2.f));
	mHpBar.setSize(sf::Vector2f(hpBarWidth, hpHeight));
	target.draw(mHpBar);

	RenderSystem::render(this, target, *getComponent<Transform>());

	//draw hitbox
	/*sf::VertexArray hitbox = sf::VertexArray(sf::Quads, 4);
	for (auto& corner : getRectangle().points)
	{
		hitbox.append(sf::Vertex(corner, sf::Color::Red));
	}
	target.draw(hitbox);*/
}

std::string Car::getActionText()
{
	std::string res = "null";
	switch (mAction)
	{
	case CarLogic::CarAction::ShootBullet:
	{
		res = "Shoot Bullets";
		break;
	}
	case CarLogic::CarAction::LaunchMissile:
	{
		res = "Launch Missile (x" + std::to_string(mMissileAmmo) + ")";
		break;
	}
	case CarLogic::CarAction::ToggleMap :
	{
		res = "Toggle Map";
		break;
	}
	default:
		break;
	}

	return res;
}

bool Car::getShowMap()
{
	return mShowMap;
}

void Car::setSprite()
{
	sf::Sprite s = sf::Sprite(mTextures.get(Textures::Car));
	s.setScale(sf::Vector2f(0.132f, 0.132f));

	sf::FloatRect bounds = s.getLocalBounds();
	s.setOrigin(bounds.width / 2, bounds.height / 2);

	Sprite sprite = Sprite(s);
	addComponent<Sprite>(sprite);
}

void Car::instanciateBullet(const sf::Vector2f& position, const sf::Vector2f& direction, std::vector<OldEntity*>& newEntities)
{
	Projectile* proj = new Projectile(1, sf::seconds(1), 1500, position + 25.f * direction, direction, sf::RectangleShape(sf::Vector2f(5, 5)), this, mTextures);
	newEntities.push_back(proj);
}
