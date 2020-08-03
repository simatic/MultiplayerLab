#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Entity.h"
//#include "SceneNode.h"


class Projectile : public Entity 
{
public:	
	enum Type
	{
		AlliedBullet,
		EnemyBullet,
		Missile,
		Laser
	};

public:
	Projectile(Type type);
	bool isGuided() const;


private:
	Type mType;
	sf::Sprite mSprite;
	sf::Vector2f mTargetDirection;
};