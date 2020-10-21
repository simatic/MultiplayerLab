#include <Car.h>
#include <Utility.h>
#include <iostream>
#include <Projectile.h>
#include <PickUp.h>
#include "ResourceHolder.h"
#include "NetworkCommon.h"
#include <math.h>


Car::Car(const TextureHolder& textures) :
	CarLogic(),
	mShowMap(false),
	mTrajectory(),
	mTextures(textures),
	mDust(sf::Color::Black, sf::Time::Zero)
{
	setSprite();
}

Car::Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, const TextureHolder& textures) :
	Car(hp, pos, rect, nullptr, textures)
{}

Car::Car(int hp, sf::Vector2f pos, sf::RectangleShape rect, KeyBinding* keys, const TextureHolder& textures) :
	CarLogic(hp, pos, rect, keys),
	mShowMap(false),
	mTrajectory(),
	mTextures(textures),
	mDust(sf::Color::Black, sf::Time::Zero)
{
	setSprite();

	mTires = sf::VertexArray(sf::Lines, 1);
	mTires[0].position = mPosition;

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

void Car::update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs)
{
	CarLogic::update(dt, entities, newEntities, pairs);

	mTires.append(sf::Vertex(mPosition - (float)20 * mCarDirection));
	mTires.append(sf::Vertex(mPosition - (float)20 * mCarDirection));

	mDust.setPosition(mPosition - (float)20 * mCarDirection);
	mDust.update(dt);

	mSprite.setPosition(mPosition);
	mSprite.setRotation(mRotation);
}

void Car::serverUpdate(sf::Time serverTime, sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs)
{
	CarLogic::serverUpdate(serverTime, dt, entities, newEntities, pairs);

	mTires.append(sf::Vertex(mPosition - (float)20 * mCarDirection));
	mTires.append(sf::Vertex(mPosition - (float)20 * mCarDirection));

	mDust.setPosition(mPosition - (float)20 * mCarDirection);
	mDust.update(dt);
}

void Car::useInputs(sf::Time dt, std::vector<Entity*>& newEntities)
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
	target.draw(mTires);
	mDust.draw(target);

	target.draw(mSprite);

	mHpBackgroundBar.setPosition(mPosition + sf::Vector2f(0, 50));
	target.draw(mHpBackgroundBar);
	float hpWidth = mHpBackgroundBar.getSize().x;
	float hpHeight = mHpBackgroundBar.getSize().y;
	float hpBarWidth = hpWidth * mHP / (float)mHpMax;
	mHpBar.setPosition(mHpBackgroundBar.getPosition() - sf::Vector2f(hpWidth / 2.f, hpHeight / 2.f));
	mHpBar.setSize(sf::Vector2f(hpBarWidth, hpHeight));
	target.draw(mHpBar);

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
	mSprite.setTexture(mTextures.get(Textures::Car)); mSprite.setScale(sf::Vector2f(0.132f, 0.132f));

	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2, bounds.height / 2);
}

void Car::instanciateBullet(const sf::Vector2f& position, const sf::Vector2f& direction, std::vector<Entity*>& newEntities)
{
	Projectile* proj = new Projectile(1, sf::seconds(1), 1500, position + 25.f * direction, direction, sf::RectangleShape(sf::Vector2f(5, 5)), this, mTextures);
	newEntities.push_back(proj);
}
