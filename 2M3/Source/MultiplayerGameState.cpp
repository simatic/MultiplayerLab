#include "MultiplayerGameState.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <fstream>

sf::IpAddress getAddressFromFile()
{
	{ // Try to open existing file (RAII block)
		std::ifstream inputFile("ip.txt");
		std::string ipAddress;
		if (inputFile >> ipAddress)
			return ipAddress;
	}

	// If open/read failed, create new file
	std::ofstream outputFile("ip.txt");
	std::string localAddress = "127.0.0.1";
	outputFile << localAddress;
	return localAddress;
}

MultiplayerGameState::MultiplayerGameState(StateStack& stack, Context context, bool isHost)
	: State(stack, context)
	, mWorld(*context.window, context.keys1, context.keys2, *context.fonts)
	, mClock()
	, mGameClient()
	, mIsHost(isHost)
	, mConnected(false)
	, mGameServer(nullptr)
{
	sf::IpAddress ip;
	if (isHost)
	{
		mGameServer.reset(new GameServer());	
		ip = "127.0.0.1";
	}
	else
	{
		ip = getAddressFromFile();
	}

	if (mGameClient.bindSocket() == sf::Socket::Done)
	{
		//Send a ID request to the server
	}

}



void MultiplayerGameState::draw()
{	
	if (mConnected){ mWorld.draw(); }
}

bool MultiplayerGameState::update(sf::Time dt)
{
	if (mConnected)
	{
		mWorld.update(dt);
		return true;
	}
	return false;
}

bool MultiplayerGameState::handleEvent(const sf::Event& event)
{
	return mWorld.handleEvent(event);
}
