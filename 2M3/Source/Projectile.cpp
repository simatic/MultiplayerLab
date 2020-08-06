#include <Projectile.h>
#include <Car.h>

Projectile::Projectile(int dmg, sf::Vector2f pos, sf::Vector2f velocity, sf::RectangleShape rect) :
	Entity(pos, rect),
	mDamage(dmg)
{
	mVelocity = velocity;
	mType = Type::ProjectileType;
}

int Projectile::getDamage()
{
	return mDamage;
}

void Projectile::onCollision(Entity* other)
{
	switch (other->getType())
	{
	case Type::ProjectileType:
	{
		remove();
		other->remove();
		break;
	}

	case Type::CarType:
	{
		Car* otherCar = dynamic_cast<Car*>(other);

		otherCar->damage(mDamage);
		remove();
		break;
	}
	}
}