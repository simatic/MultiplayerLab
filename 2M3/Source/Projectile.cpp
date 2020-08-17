#include <Projectile.h>
#include <Car.h>

Projectile::Projectile(int dmg, sf::Vector2f pos, sf::Vector2f velocity, sf::RectangleShape rect, Car* car, sf::Time lifetime, const TextureHolder& textures) :
	Entity(pos, rect, textures),
	mDamage(dmg),
	mCar(car),
	mLifetime(lifetime)
{
	mVelocity = velocity;
	mType = Type::ProjectileType;
	Entity::setSprite();
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

void Projectile::update(sf::Time dt, std::vector<Entity*> entities, std::vector<Entity*>& newEntities, std::set<Pair>& pairs)
{
	mLifetime -= dt;
	if (mLifetime < sf::Time::Zero)
	{
		mToRemove = true;
	}
	mRotation = mCar->getRotation();
	Entity::update(dt, entities, newEntities, pairs);
}