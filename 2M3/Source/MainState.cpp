#include "MainState.h"
#include <Common/Network/Constants.h>

#include <imgui.h>
#include <imgui-SFML.h>

MainState::ClientInfo::ClientInfo(int uid, sf::Mutex& mutex, int renderTextureWidth, int renderTextureHeight, sf::Thread* thread) :
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
	client = std::make_unique<Client>(uid, mutex, renderTextureWidth, renderTextureHeight, queueToDraw, queueToDisplay);
}

MainState::MainState(StateStack& stack, Context context, sf::RenderWindow* window, Settings* settings) :
	State(stack, context),
	_clientCount(0),
	_mutex(std::make_unique<sf::Mutex>()),
	_clientThreadLaunchedIndex(0),
	window(window),
	settings(settings)
{
	ImGui::SFML::Init(*window);
	launchServer();
	addClientThread();
	addClientThread();
	for (size_t i = 0; i < _clientsInfo.size(); i++) {
		if (_clientsInfo[i]->thread != nullptr) {
			_clientsInfo[i]->thread->launch();
			std::cout << "Launch the thread of Client " << i << "." << std::endl;
		}
	}
}

bool MainState::update(sf::Time dt) {
	if (settings->changed) {
		_clientsInfo[0]->client->setKeyBinding(settings->playerAKeyBinding);
		_clientsInfo[1]->client->setKeyBinding(settings->playerBKeyBinding);
	}

	bool serverStillRunning = false;
	if (auto server = serverReference.lock()) {
		serverStillRunning = server->isRunning();
	}

	if (!serverStillRunning) {
		_mutex->lock();
		//quitApplication();
		_mutex->unlock();
	}

	_clientInputs.clear();

	return true;
}

bool MainState::handleEvent(const sf::Event& event) {
	ImGui::SFML::ProcessEvent(event);
	_clientInputs.push_back(event);

	for (size_t i = 0; i < _clientsInfo.size(); i++) {
		if (_clientsInfo[i]->client != nullptr) {
			_mutex->lock();
			_clientsInfo[i]->client->setInputs(_clientInputs);
			_mutex->unlock();
		}
	}

	return true;
}

void MainState::draw() {
	const float clientWidth = window->getView().getSize().x / 2.0f;
	const float startY = window->getSize().y * gameHeightRatio;
	const float clientHeight = window->getSize().y * (1.0f - gameHeightRatio);
	sf::Sprite spriteToDisplay[2];
	for (size_t i = 0; i < _clientsInfo.size(); i++)
	{
		{
			_clientsInfo[i]->queueToDisplay.wait_and_pop(spriteToDisplay[i]);
			ZoneScopedN("Present client to screen");
			window->draw(spriteToDisplay[i]);
		}
	}
	serverInterface->render(*window, clientWidth, clientHeight, startY);
	window->display();

	for (size_t i = 0; i < _clientsInfo.size(); i++) {
		_clientsInfo[i]->queueToDraw.push(spriteToDisplay[i]);
	}
}

void MainState::addClientThread() {
	_clientsInfo.push_back(std::make_unique<ClientInfo>(_clientCount, *(_mutex.get()), window->getSize().x / 2, window->getSize().y * gameHeightRatio, new sf::Thread(&MainState::launchClientThread, this)));
	++_clientCount;
}

void MainState::launchClientThread() {
	_mutex->lock();
	int clientUID = _clientThreadLaunchedIndex;
	_clientThreadLaunchedIndex++;
	Client* client(_clientsInfo[clientUID]->client.get());
	_mutex->unlock();
	if (clientUID == 0) {
		client->initialize(settings->playerAKeyBinding);
	}
	else {
		sf::sleep(sf::seconds(0.01));
		client->initialize(settings->playerBKeyBinding);
	}
	client->run();
}

void MainState::terminateClientThreads() {
	// Wait for each threads to terminate.
	{
		for (size_t i = 0; i < _clientsInfo.size(); i++) {
			if (_clientsInfo[i]->client != nullptr) {
				_clientsInfo[i]->client->terminate();
				_clientsInfo[i]->queueToDraw.StopWaiting();
				_clientsInfo[i]->thread->wait();
				std::cout << "Thread " << i << " ended." << std::endl;
			}
		}
	}
}

void MainState::launchServer() {
	std::cout << "Launching server thread" << std::endl;
	auto server = std::make_shared<Server>("0.0.0.0", DEFAULT_PORT);
	serverReference = server;
	serverInterface = std::make_unique<Interface>(server->getNetworkHandler());
	serverThread = std::thread([server /* copy server shared_ptr into thread*/]() {
		server->init();
		// TODO: configurable host
		server->run();
		});
	while (!server->isReady()) std::this_thread::yield();
}

void MainState::terminateServer() {
	if (auto server = serverReference.lock()) {
		std::cout << "Stopping server thread" << std::endl;
		server->stop();
	}

	std::cout << "Waiting on server thread death" << std::endl;
	serverThread.join();
}

MainState::~MainState() {
	terminateServer();
	terminateClientThreads();
}

