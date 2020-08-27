#include <GameServer.h>
#include <NetworkCommon.h>
#include <iostream>

GameServer::GameServer(const TextureHolder& textures)
	: mWorld(textures)
	, mClock()
	, mSocket()
	, mClients()
	, mThread(&GameServer::run, this)
	, mStopThread(false)
{
	mPort = ServerPort;
	mAdress	= sf::IpAddress::getLocalAddress();
	bindPort();
	mAvailableEntityIDs.push(1);
	mAvailableClientIDs.push(0);
}

void GameServer::run()
{
	sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
	sf::Time timeSinceLastFrame = sf::Time::Zero;
	sf::Time timeSinceLastTick = sf::Time::Zero;
	sf::Time lastDate = mClock.getElapsedTime();

	while (mHostWindow->isOpen() && !mStopThread)
	{
		sf::Time t = mClock.getElapsedTime();
		sf::Time dt = t - lastDate;
		lastDate = t;
		timeSinceLastTick += dt;
		timeSinceLastFrame += dt;

		processWaitingPackets();

		while (timeSinceLastTick > TimePerTick)
		{
			timeSinceLastTick -= TimePerTick;
			//std::cout << "server tick" << std::endl;

			auto cars = mWorld.getCars();

			for (auto& client : mClients)
			{
				sendPing(client);
				sendCarsUpdate(client, cars);
			}
		}

		while (timeSinceLastFrame > TimePerFrame)
		{
			timeSinceLastFrame -= TimePerFrame;

			mWorld.update(t, dt);
		}
	}
}

void GameServer::start(sf::RenderWindow* window)
{
	mHostWindow = window;
	mThread.launch();
}

void GameServer::stop()
{
	mStopThread = true;
	mThread.wait();
}

void GameServer::processWaitingPackets()
{
	sf::Socket::Status status;
	do
	{
		// We try to see if there is a message to process
		sf::Packet packet;
		sf::IpAddress remoteAddress;
		unsigned short remotePort;
		status = mSocket.receive(packet, remoteAddress, remotePort);
		if (status == sf::Socket::NotReady || status == sf::Socket::Disconnected)
			break;

		// We process the message
		processReceivedPacket(packet, remoteAddress, remotePort);

	} while (true); // We exit this loop thanks to break instruction when ((status == sf::Socket::NotReady) || (status == sf::Socket::Disconnected))
}

void GameServer::processReceivedPacket(sf::Packet& packet, sf::IpAddress& remoteAddress, unsigned short remotePort)
{
	ClientMsgType messageType;
	packet >> messageType;

	switch (messageType)
	{
	case ClientMsgType::ClientIdRequest:
	{
		sf::Uint32 newID = getNewClientID();
		ClientData client = ClientData(newID, remoteAddress, remotePort);
		mClients.push_back(client);

		EntityStruct p1 = { getNewEntityID(), Entity::Type::CarType, sf::Vector2f(8000, 4500), sf::Vector2f(0, 0) };
		EntityStruct p2 = { getNewEntityID(), Entity::Type::CarType, sf::Vector2f(8100, 4500), sf::Vector2f(0, 0) };

		mWorld.createCar(p1);
		mWorld.createCar(p2);
		
		sf::Packet toSend;
		toSend << ServerMsgType::ClientIdResponse << newID << mClock.getElapsedTime() << p1 << p2;
		mSocket.send(toSend, remoteAddress, remotePort);

		for (auto& cl : mClients)
		{
			if (cl.getID() != newID)
			{
				toSend.clear();
				toSend << ServerMsgType::ObjectCreation << p1;
				mSocket.send(toSend, cl.getAddress(), cl.getPort());

				toSend.clear();
				toSend << ServerMsgType::ObjectCreation << p2;
				mSocket.send(toSend, cl.getAddress(), cl.getPort());
			}
		}

		break;
	}
	case ClientMsgType::Input:
	{
		sf::Uint64 id;
		Inputs inputs;
		packet >> id >> inputs;

		//std::cout << "serv received inputs of car " << id << std::endl;

		mWorld.setCarInputs(id, inputs, mClock.getElapsedTime());

		break;
	}
	case ClientMsgType::PingResponse:
	{
		sf::Uint32 id;
		sf::Time timeSent;
		packet >> id >> timeSent;

		//std::cout << "serv received ping response by " << id << std::endl;

		ClientData& client = getClientFromID(id);
		client.setDelay(sf::microseconds((mClock.getElapsedTime() - timeSent).asMicroseconds() / 2));

		break;
	}
	default:
		break;
	}
}

sf::Uint64 GameServer::getNewEntityID()
{
	sf::Uint64 id = mAvailableEntityIDs.top();
	mAvailableEntityIDs.pop();
	if (mAvailableEntityIDs.empty()) mAvailableEntityIDs.emplace(id + 1);
	return id;
}

sf::Uint32 GameServer::getNewClientID()
{
	sf::Uint32 id = mAvailableClientIDs.top();
	mAvailableClientIDs.pop();
	if (mAvailableClientIDs.empty()) mAvailableClientIDs.emplace(id + 1);
	return id;
}

ClientData& GameServer::getClientFromID(sf::Uint32 id)
{
	for (auto& client : mClients)
	{
		if (client.getID() == id) return client;
	}
	std::cerr << "Error: no client with such ID : " << id << std::endl;
	exit(EXIT_FAILURE);
}

void GameServer::sendPing(ClientData& client)
{
	//std::cout << "serv ping at " << client.getID() << std::endl;
	sf::Packet packet;
	packet << ServerMsgType::PingRequest << mClock.getElapsedTime();
	mSocket.send(packet, client.getAddress(), client.getPort());
}

void GameServer::sendCarsUpdate(ClientData& client, const std::vector<Entity*>& cars)
{
	sf::Packet packet;
	for (auto& carEnt : cars)
	{
		packet << ServerMsgType::CarUpdate;

		EntityStruct carStruct = { carEnt->getID(), Entity::Type::CarType, carEnt->getPosition(), carEnt->getVelocity() };
		packet << carStruct;

		Car* car = dynamic_cast<Car*>(carEnt);
		packet << car->getCarDirection();

		mSocket.send(packet, client.getAddress(), client.getPort());
		packet.clear();
	}

}

sf::Socket::Status GameServer::bindPort()
{
	return mSocket.bind(mPort);
}

const sf::IpAddress& GameServer::getAdress()
{
	const sf::IpAddress& t = mAdress;
	return t;
}

const unsigned short& GameServer::getPort()
{
	const unsigned short& t = mPort;
	return t;
}

void GameServer::setSocketBlocking(bool isBlocking)
{
	mSocket.setBlocking(isBlocking);
}