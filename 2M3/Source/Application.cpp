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
	_mutex(std::make_unique<sf::Mutex>()),
	_clientThreadLaunchedIndex(0)
{
	mWindow.setKeyRepeatEnabled(false);
	mWindow.setVerticalSyncEnabled(true);
	_isMainWindowOpen = mWindow.isOpen();
}

Application::ClientInfo::ClientInfo(Client* client, sf::Thread* thread, /*const sf::Texture& texture,*/ sf::Sprite* sprite) :
	client(client),
	thread(thread),
	//texture(&texture),
	sprite(sprite)
{
}

void Application::run()
{
	addClientThread();
	addClientThread();
	/*for (size_t i = 0; i < _clientThreads.size(); i++)
	{
		if (_clientThreads[i] != nullptr) {
			_clientThreads[i]->launch();
			std::cout << "Launch the thread of Client " << i << "." << std::endl;
		}
	}*/
	for (size_t i = 0; i < _clientsInfo.size(); i++)
	{
		if (_clientsInfo[i]->thread != nullptr) {
			_clientsInfo[i]->thread->launch();
			std::cout << "Launch the thread of Client " << i << "." << std::endl;
		}
	}

	while (_isMainWindowOpen) {
		manageInputs();
		sf::sleep(sf::seconds(TimePerFrame.asSeconds()));
		render();
	}
}

void Application::addClientThread() {
	sf::RenderTexture* clientRenderTexture(new sf::RenderTexture());
	// Necessary to be called after initialization of RenderTexture.
	clientRenderTexture->create(mWindow.getSize().x / 2, mWindow.getSize().y);
	sf::Sprite* sprite = new sf::Sprite(clientRenderTexture->getTexture());
	if (_clientCount == 1) {
		sprite->move(mWindow.getSize().x / 2, 0);
	}
	Client* client = new Client(_clientCount, mWindow, *_mutex, clientRenderTexture);
	_clientsInfo.push_back(std::make_unique<ClientInfo>(client, new sf::Thread(&Application::launchClientThread, this), sprite));

	// TODO delete in a near future.
	/*_clients.push_back(std::shared_ptr<Client>(new Client(_clientCount, mWindow, *_mutex, clientRenderTexture)));
	_clientThreads.push_back(std::unique_ptr<sf::Thread>(new sf::Thread(&Application::launchClientThread, this)));*/
	// End of delete

	++_clientCount;
}

void Application::launchClientThread() {
	_mutex->lock();
	int clientUID = _clientThreadLaunchedIndex;
	_clientThreadLaunchedIndex++;
	//std::shared_ptr<Client> client(_clients[clientUID]);
	Client* client(_clientsInfo[clientUID]->client.get());
	_mutex->unlock();
	if (clientUID == 0) {
		client->initialize(2);
	}
	else
	{
		sf::sleep(sf::seconds(0.01));
		client->initialize(1);
	}
	client->run();
}

void Application::terminateClientThreads() {
	mWindow.setActive(false);
	_mutex->unlock();
	// Wait for each threads to terminate.
	{
		/*for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i] != nullptr) {
				_clients[i]->terminate();
				_clientThreads[i]->wait();
				std::cout << "Thread " << i << " ended.\n";
			}
		}*/
		for (size_t i = 0; i < _clientsInfo.size(); i++)
		{
			if (_clientsInfo[i]->client != nullptr) {
				_clientsInfo[i]->client->terminate();
				_clientsInfo[i]->thread->wait();
				std::cout << "Thread " << i << " ended.\n";
			}
		}
	}
}

void Application::manageInputs() {
	_clientInputs.clear();
	sf::Event event;
	_mutex->lock();
	mWindow.setActive(true);
	while (mWindow.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
		{
			terminateClientThreads();
			mWindow.close();
			_isMainWindowOpen = false;
		}
		else
		{
			_clientInputs.push_back(event);
		}
	}
	mWindow.setActive(false);
	_mutex->unlock();
	/*for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i] != nullptr) {
			_clients[i]->setInputs(_clientInputs);
		}
	}*/
	for (size_t i = 0; i < _clientsInfo.size(); i++) {
		if (_clientsInfo[i]->client != nullptr) {
			_clientsInfo[i]->client->setInputs(_clientInputs);
		}
	}
}

void Application::render()
{
	// TODO if mWindow is not accessed from other threads anymore, move _mutex in for loop.
	// TODO do the TODO above when _mainWindow is replaced by _renderTexture in Client.
	_mutex->lock();
	// Seems necessary when there are multiple contexts.
	mWindow.setActive(true);
	mWindow.clear();

	for (size_t i = 0; i < _clientsInfo.size(); i++)
	{ 
		mWindow.draw(*_clientsInfo[i]->sprite);
	}

	mWindow.display();
	mWindow.setActive(false);
	_mutex->unlock();
}