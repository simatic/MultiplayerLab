#include "Application.h"
#include "Utility.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "TitleState.h"
#include "MenuState.h"
#include "SettingsState.h"
#include "GameState.h"
#include "MultiplayerGameState.h"
#include "NetworkCommon.h"

#include <iostream>

Application::Application() :
	mWindow(sf::VideoMode::getDesktopMode()/*sf::VideoMode(1600, 900)*/, "2M3", sf::Style::Close | sf::Style::Resize),
	_clientCount(0),
	_mutex(),
	_clientThreadLaunchedIndex(0)
{
	mWindow.setKeyRepeatEnabled(false);
	mWindow.setVerticalSyncEnabled(true);
}

void Application::run()
{
	addClientThread();
	addClientThread();
	std::cout << "Launch the thread of Client 1.\n";
	_clientThreads[0]->launch();
	std::cout << "Launch the thread of Client 2.\n";
	_clientThreads[1]->launch();
	while (mWindow.isOpen()) {
		sf::Event event;
		while (mWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
			{
				terminateClientThreads();
				mWindow.close();
			}
		}
	}
}

void Application::addClientThread() {
	std::string title;
	if (_clientCount == 0) {
		title = "Client 1";
	}
	else
	{
		title = "Client 2";
	}
	_clients.push_back(new Client(_clientCount, &mWindow, title, &_mutex));
	_clientThreads.push_back(new sf::Thread(&Application::launchClientThread, this));
	++_clientCount;
}

void Application::launchClientThread() {
	Client* client;
	_mutex.lock();
	int clientUID = _clientThreadLaunchedIndex;
	_clientThreadLaunchedIndex++;
	client = _clients[clientUID];
	_mutex.unlock();
	if (clientUID == 0) {
		client->initialize(2);
	}
	else
	{
		sf::sleep(sf::seconds(0.1));
		client->initialize(1);
	}
	client->run();
}

void Application::terminateClientThreads() {
	// Wait for each threads to terminate.
	{
		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i] != nullptr) {
				_clients[i]->terminate();
			}
		}
	}
	_clientThreads[0]->wait();
	std::cout << "First thread ended.\n";
	_clientThreads[1]->wait();
	std::cout << "Second thread ended.\n";
}