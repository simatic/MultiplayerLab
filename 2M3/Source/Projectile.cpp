#include <Projectile.h>
#include <Car.h>

Projectile::Projectile(int dmg, sf::Vector2f pos, sf::Vector2f velocity, sf::RectangleShape rect, Car* car) :
	Entity(pos, rect),
	mDamage(dmg),
	mCar(car)
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
		if (otherCar == mCar)
		{
			break;
		}

		otherCar->damage(mDamage);
		remove();
		break;
	}
	}
}

Car* Projectile::getCar()
{
	return mCar;
}