#include "Entity.h"
#include "Projectile.h"
//#include "SceneNode.h"

#include <SFML/System.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>

Projectile::Projectile(Type type) : Entity(1)
{
	mType = type;
}

bool Projectile::isGuided() const
{
	return mType == Missile;
}

//bool Projectile::guideTowards(sf::Vector2f position)
//{
//	mTargetDirection = sf::Vector2f(position - getWorldPosition());
//}



	