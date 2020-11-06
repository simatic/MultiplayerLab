#include "Client.h"
#include "TitleState.h"
#include "MenuState.h"
#include "SettingsState.h"
#include "GameState.h"
#include "MultiplayerGameState.h"

Client::Client(int uid, sf::RenderWindow* mainWindow, std::string clientTitle, std::shared_ptr <sf::Mutex> mutex) :
	_uid(uid),
	_mainWindow(/*mainWindow*/ /* TODO uncomment for having 1 window */ nullptr),
	_applicationMutex(mutex),
	_isThreadTerminated(false),
	_view(),
	_textures(nullptr),
	_fonts(nullptr),
	_keybinding(nullptr),
	_stateStack(nullptr),
	_statisticsText(),
	_statisticsUpdateTime(),
	_statisticsNumFrames(0)
{
	/* TODO uncomment when removing separate windows.
	sf::Vector2f mainWindowSize(_mainWindow->getSize());
	_view.setSize(mainWindowSize.x / 2, mainWindowSize.y);
	if (uid == 0) {
		_view.setViewport(sf::FloatRect(0, 0, 0.5f, 1.0f));
	}
	else {
		_view.setViewport(sf::FloatRect(0.5f, 0, 0.5f, 1.0f));
	}*/
}

void Client::initialize(int keyBindingConfiguration) {
	/* TODO Comment the 3 lines about _mainWindow when adding View*/
	_mainWindow.reset(new sf::RenderWindow(sf::VideoMode::getDesktopMode(), toString(_uid), sf::Style::Close | sf::Style::Resize));
	_mainWindow->setKeyRepeatEnabled(false);
	_mainWindow->setVerticalSyncEnabled(true);

	_textures.reset(new TextureHolder());
	_fonts.reset( new FontHolder());
	
	_fonts->load(Fonts::Main, "Media/Sansation.ttf");

	_textures->load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");
	_textures->load(Textures::MenuScreen, "Media/Textures/MenuScreen.png");
	_textures->load(Textures::Buttons, "Media/Textures/Button.png");

	_statisticsText.setFont(_fonts->get(Fonts::Main));
	_statisticsText.setPosition(5.f, 5.f);
	_statisticsText.setCharacterSize(10u);

	_keybinding.reset(new KeyBinding(keyBindingConfiguration));
	_stateStack.reset(new StateStack(State::Context(*_mainWindow, *_textures, *_fonts, *_keybinding)));
	registerStates();
	_stateStack->pushState(States::Title);
}

void Client::run() {
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timeSinceLastTick = sf::Time::Zero;

	bool isThreadTerminated;

	while (_mainWindow->isOpen())
	{
		_applicationMutex->lock();
		isThreadTerminated = _isThreadTerminated;
		_applicationMutex->unlock();
		if (isThreadTerminated) {
			if (_mainWindow->isOpen()) {
				_mainWindow->close();
				break;
			}
		}
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (_stateStack->isEmpty())
				_mainWindow->close();
		}

		timeSinceLastTick += dt;
		while (timeSinceLastTick > TimePerTick)
		{
			timeSinceLastTick -= TimePerTick;

			tick();
		}

		updateStatistics(dt);
		render();
	}
}

void Client::terminate() {
	_applicationMutex->lock();
	_isThreadTerminated = true;
	_applicationMutex->unlock();
}

void Client::processInput()
{
	sf::Event event;
	while (_mainWindow->pollEvent(event))
	{
		_stateStack->handleEvent(event);

		if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			_mainWindow->close();
	}
}

void Client::update(sf::Time dt)
{
	_stateStack->update(dt);
}

void Client::tick()
{
	_stateStack->tick();
}

void Client::render()
{
	_mainWindow->clear();

	_stateStack->draw();

	_mainWindow->setView(_mainWindow->getDefaultView());
	_mainWindow->draw(_statisticsText);

	_mainWindow->display();
}

void Client::updateStatistics(sf::Time dt)
{
	_statisticsUpdateTime += dt;
	_statisticsNumFrames += 1;
	if (_statisticsUpdateTime >= sf::seconds(1.0f))
	{
		_statisticsText.setString("FPS: " + toString(_statisticsNumFrames));

		_statisticsUpdateTime -= sf::seconds(1.0f);
		_statisticsNumFrames = 0;
	}
}

void Client::registerStates()
{
	_stateStack->registerState<TitleState>(States::Title);
	_stateStack->registerState<MenuState>(States::Menu);
	_stateStack->registerState<GameState>(States::Game);
	_stateStack->registerState<SettingsState>(States::Settings);
	_stateStack->registerState<MultiplayerGameState>(States::HostGame, true);
	_stateStack->registerState<MultiplayerGameState>(States::JoinGame, false);
}