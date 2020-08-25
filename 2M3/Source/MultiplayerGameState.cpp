#include "MultiplayerGameState.h"
#include "NetworkCommon.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <fstream>
#include <iostream>

MultiplayerGameState::MultiplayerGameState(StateStack& stack, Context context, bool isHost)
	: State(stack, context)
	, mWorld(*context.window, context.keys1, context.keys2, *context.fonts)
	, mClock()
	, mGameClient()
	, mIsHost(isHost)
	, mConnected(false)
	, mGameServer(nullptr)
{
	if (isHost)
	{
		mGameServer.reset(new GameServer());
		std::cout << "Server address: " << mGameServer->getAdress() <<"\n";
	}
	if (mGameClient.bindSocket() != sf::Socket::Done)
	{
		std::cout << "Could not bind Socket\n";
	}
	if (!isHost)
	{
		std::string address;
		std::cout << "Input server address: \n";
		std::cin >> address;
		sf::IpAddress serverAddress(address);
		mGameClient.setServerAddress(serverAddress);	

		sf::Packet packet;
		packet << ClientMsgType::ClientIdRequest;
		mGameClient.sendPacket(packet, serverAddress, 5000);
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
		mWorld.clientUpdate(dt);
		return true;
	}
	return false;
}

bool MultiplayerGameState::handleEvent(const sf::Event& event)
{
	return mWorld.handleEvent(event);
}


//sf::ipaddress getaddressfromfile()
//{
//	{ // try to open existing file (raii block)
//		std::ifstream inputfile("ip.txt");
//		std::string ipaddress;
//		if (inputfile >> ipaddress)
//			return ipaddress;
//	}
//
//	// if open/read failed, create new file
//	std::ofstream outputfile("ip.txt");
//	std::string localaddress = "127.0.0.1";
//	outputfile << localaddress;
//	return localaddress;
//}