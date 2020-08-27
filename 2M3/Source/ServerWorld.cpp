#include <ServerWorld.h>
#include <Car.h>
#include <Projectile.h>
#include <PickUp.h>
#include <functional>
#include <iostream>

ServerWorld::ServerWorld(const TextureHolder& textures)
	: mWorldWidth(16000.f)
	, mWorldHeight(9000.f)
	, mTextures(textures)
{
}

void ServerWorld::update(sf::Time serverTime, sf::Time dt, sf::UdpSocket& socket, std::vector<ClientData>& clients)
{
	for (auto& ent : mEntities)
	{
		ent->serverUpdate(serverTime, dt, mEntities, mNewEntities, mPairs);
	}

	for (auto& pair : mPairs)
	{
		pair.first->onCollision(pair.second);
		//to do : send object collisions
		for (auto& client : clients)
		{
			sf::Packet packet;
			packet << pair.first->getID() << pair.second->getID();
			std::cout << "sending collision between " << pair.first->getID() << " and " << pair.second->getID() << std::endl;
			socket.send(packet, client.getAddress(), client.getPort());
		}
	}
	auto removeBegin = std::remove_if(mEntities.begin(), mEntities.end(), std::mem_fn(&Entity::toRemove));
	mEntities.erase(removeBegin, mEntities.end());

	for (auto& ent : mEntities)
	{
		ent->cleanUp(getWorldSize(), dt);
	}

	for (auto& newEnt : mNewEntities)
	{
		mEntities.push_back(newEnt);
		//to do : send object creations
		for (auto& client : clients)
		{
			sf::Packet packet;
			Entity::Type type = newEnt->getType();
			EntityStruct entStruct = { newEnt->getID(), type, newEnt->getPosition(), newEnt->getVelocity() };
			packet << ServerMsgType::ObjectCreation << entStruct;

			switch (type)
			{
			case Entity::Type::CarType:
			{
				Car* car = dynamic_cast<Car*>(newEnt);
				packet << car->getCarDirection();

				break;
			}
			case Entity::Type::ProjectileType:
			{
				Projectile* proj = dynamic_cast<Projectile*>(newEnt);
				packet << proj->getCar()->getID() << proj->isGuided();

				break;
			}
			case Entity::Type::PickUpType:
				break;
			default:
				break;
			}
			//socket.send(packet, client.getAddress(), client.getPort());
		}
	}
	mNewEntities.clear();
	mPairs.clear();
}

bool ServerWorld::handleEvent(const sf::Event& event)
{
	bool res = true;
	for (auto ent : mEntities)
	{
		res = ent->handleEvent(event) && res;
	}

	return res;
}

sf::Vector2f ServerWorld::getWorldSize()
{
	return sf::Vector2f(mWorldWidth, mWorldHeight);
}

std::vector<Entity*> ServerWorld::getCars()
{
	std::vector<Entity*> res;
	for (auto& ent : mEntities)
	{
		if (ent->getType() == Entity::Type::CarType)
		{
			res.push_back(ent);
		}
	}
	return res;
}

Entity* ServerWorld::getEntityFromId(sf::Uint64 id)
{
	std::cout << "searching for entity id " << id << std::endl;
	for (auto& ent : mEntities)
	{
		if (ent->getID() == id) return ent;
	}
	for (auto& ent : mNewEntities)
	{
		if (ent->getID() == id) return ent;
	}
	std::cerr << "Error: no entity with such ID : " << id << std::endl;
	exit(EXIT_FAILURE);
}

//bool operator<(const TimedInputs& inputs1, const TimedInputs& inputs2)
//{
//	return inputs1.timestamp < inputs2.timestamp;
//}

void ServerWorld::setCarInputs(sf::Uint64 id, Inputs inputs, sf::Time t)
{
	std::cout << "setting car inputs " << id << std::endl;
	Entity* entity = getEntityFromId(id);
	Car* car = dynamic_cast<Car*>(entity);
	car->insertInputs(t, inputs);

	//mInputs.insert({ id, t, inputs });
	
}

void ServerWorld::rollback(sf::Time present, sf::Time rollbackDate, sf::UdpSocket& socket, std::vector<ClientData>& clients)
{
	sf::Time current = present;
	std::stack<sf::Time> deltas = std::stack<sf::Time>();

	while (current > rollbackDate)
	{
		UpdateFrame frame = mFrames.top();
		mFrames.pop();

		current -= frame.dt;
		deltas.push(frame.dt);

		for (auto& up : frame.velocityUpdates)
		{
			up.entity->offset(up.entity->getVelocity() * frame.dt.asSeconds());
			up.entity->setVelocity(up.oldVelocity);
		}

		for (auto& up : frame.presenceUpdates)
		{
			if (up.added)
			{
				remove(mEntities.begin(), mEntities.end(), up.entity);
			}
			else
			{
				up.entity->unremove();
				mEntities.push_back(up.entity);
			}
		}
	}

	while (current < present)
	{
		sf::Time dt = deltas.top();
		deltas.pop();

		update(current, dt, socket, clients);
		current += dt;
	}
}

void ServerWorld::createCar(EntityStruct car)
{
	std::cout << "creating car " << car.id << std::endl;
	Car* obj = new Car(100, car.position, sf::RectangleShape(sf::Vector2f(80, 40)), mTextures);
	obj->setID(car.id);
	mNewEntities.push_back(obj);
}

void ServerWorld::sendWorld(ClientData client, sf::UdpSocket& socket, sf::Uint64 idCar1, sf::Uint64 idCar2)
{
	std::cout << "sending world" << std::endl;
	for (auto& ent : mEntities)
	{
		sf::Uint64 id = ent->getID();
		if (id != idCar1 && id != idCar2)
		{
			Entity::Type type = ent->getType();
			EntityStruct entStruct = { id, type, ent->getPosition(), ent->getVelocity() };

			sf::Packet packet;
			packet << ServerMsgType::ObjectCreation << entStruct;

			switch (type)
			{
			case Entity::Type::CarType:
			{
				Car* car = dynamic_cast<Car*>(ent);
				packet << car->getCarDirection();

				break;
			}
			case Entity::Type::ProjectileType:
			{
				Projectile* proj = dynamic_cast<Projectile*>(ent);
				packet << proj->getCar()->getID() << proj->isGuided();

				break;
			}
			case Entity::Type::PickUpType:
				break;
			default:
				break;
			}

			socket.send(packet, client.getAddress(), client.getPort());
		}
	}
}
