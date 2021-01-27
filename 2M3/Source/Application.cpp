#include "Application.h"
#include "Utility.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "TitleState.h"
#include "MenuState.h"
#include "SettingsState.h"
#include "GameState.h"

#include <iostream>
#include <Server/Server.h>
#include <Server/Interface.h>
#include <Common/Network/Constants.h>
#include <imgui-SFML.h>
#include "Profiling.h"

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __linux__
// TODO
#endif

#ifdef __APPLE__
    // TODO
#endif

Application::Application() :
	mWindow(sf::VideoMode::getDesktopMode()/*sf::VideoMode(1600, 900)*/, "2M3", sf::Style::Close | sf::Style::Resize),
	_clientCount(0),
	_mutex(std::make_unique<sf::Mutex>()),
	_clientThreadLaunchedIndex(0)
{
	mWindow.setKeyRepeatEnabled(false);
	mWindow.setVerticalSyncEnabled(true);

	// maximize window on launch

#ifdef WIN32
    ::ShowWindow(mWindow.getSystemHandle(), SW_MAXIMIZE);
#endif
#ifdef __linux__
    // TODO
#endif

#ifdef __APPLE__
    // TODO
#endif
	// TODO remove this because it became useless
	_isMainWindowOpen = mWindow.isOpen();
}

Application::ClientInfo::ClientInfo(int uid, sf::Mutex& mutex, int renderTextureWidth, int renderTextureHeight, sf::Thread* thread) :
	client(),
	thread(thread),
	queueToDraw(),
	queueToDisplay()
{
	sf::Sprite sprite = sf::Sprite();
	if (uid == 1) {
		sprite.move(renderTextureWidth, 0);
	}
	queueToDraw.push(sprite);
	client = std::make_unique <Client>(uid, mutex, renderTextureWidth, renderTextureHeight, queueToDraw, queueToDisplay);
}

void Application::run()
{
    launchServer();
	addClientThread();
	addClientThread();
	for (size_t i = 0; i < _clientsInfo.size(); i++)
	{
		if (_clientsInfo[i]->thread != nullptr) {
			_clientsInfo[i]->thread->launch();
			std::cout << "Launch the thread of Client " << i << "." << std::endl;
		}
	}

	while (_isMainWindowOpen) {
	    FrameMark;
	    bool serverStillRunning = false;
        if(auto server = serverReference.lock()) {
            serverStillRunning = server->isRunning();
        }

        if(!serverStillRunning) {
            _mutex->lock();
            quitApplication();
            _mutex->unlock();
        }

		manageInputs();
		//sf::sleep(sf::seconds(TimePerFrame.asSeconds()));
		render();
	}
}

void Application::addClientThread() {
	_clientsInfo.push_back(std::make_unique<ClientInfo>(_clientCount, *(_mutex.get()), mWindow.getSize().x / 2, mWindow.getSize().y * gameHeightRatio, new sf::Thread(&Application::launchClientThread, this)));
	++_clientCount;
}

void Application::launchClientThread() {
	_mutex->lock();
	int clientUID = _clientThreadLaunchedIndex;
	_clientThreadLaunchedIndex++;
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
	// Wait for each threads to terminate.
	{
		for (size_t i = 0; i < _clientsInfo.size(); i++)
		{
			if (_clientsInfo[i]->client != nullptr) {
				_clientsInfo[i]->client->terminate();
				_clientsInfo[i]->queueToDraw.StopWaiting();
				_clientsInfo[i]->thread->wait();
				std::cout << "Thread " << i << " ended." << std::endl;
			}
		}
	}
}

void Application::manageInputs() {
	_clientInputs.clear();
	sf::Event event;
	while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
		{
		    quitApplication();
		}
		else
		{
            ImGui::SFML::ProcessEvent(event);
            _clientInputs.push_back(event);
		}
	}
	for (size_t i = 0; i < _clientsInfo.size(); i++) {
		if (_clientsInfo[i]->client != nullptr) {
			_mutex->lock();
			_clientsInfo[i]->client->setInputs(_clientInputs);
			_mutex->unlock();
		}
	}
}

void Application::render()
{
	mWindow.clear();

	const float clientWidth = mWindow.getView().getSize().x / 2.0f;
	const float startY = mWindow.getSize().y*gameHeightRatio;
	const float clientHeight = mWindow.getSize().y*(1.0f-gameHeightRatio);
    sf::Sprite spriteToDisplay[2];
    for (size_t i = 0; i < _clientsInfo.size(); i++)
	{ 
        {
            _clientsInfo[i]->queueToDisplay.wait_and_pop(spriteToDisplay[i]);
            ZoneScopedN("Present client to screen");
            mWindow.draw(spriteToDisplay[i]);
        }
    }
    serverInterface->render(mWindow, clientWidth, clientHeight, startY);
	mWindow.display();

    for (size_t i = 0; i < _clientsInfo.size(); i++) {
        _clientsInfo[i]->queueToDraw.push(spriteToDisplay[i]);
    }
}

void Application::launchServer() {
    std::cout << "Launching server thread" << std::endl;
    auto server = std::make_shared<Server>("0.0.0.0", DEFAULT_PORT);
    serverReference = server;
    serverInterface = std::make_unique<Interface>(server->getNetworkHandler());
    serverThread = std::thread([server /* copy server shared_ptr into thread*/]() {
        server->init();
        // TODO: configurable host
        server->run();
    });
    while(!server->isReady()) std::this_thread::yield();
}

void Application::terminateServer() {
    if(auto server = serverReference.lock()) {
        std::cout << "Stopping server thread" << std::endl;
        server->stop();
    }

    std::cout << "Waiting on server thread death" << std::endl;
    serverThread.join();
}

void Application::quitApplication() {
    terminateServer();
    terminateClientThreads();
    mWindow.close();
    _isMainWindowOpen = false;
}

