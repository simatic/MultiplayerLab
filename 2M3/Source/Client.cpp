#include "Client.h"
#include "TitleState.h"
#include "MenuState.h"
#include "SettingsState.h"
#include "GameState.h"
#include "Utility.h"


Client::Client(int uid, sf::RenderWindow& mainWindow, sf::Mutex& mutex, sf::RenderTexture* renderTexture) :
	_uid(uid),
	_mainWindow(&mainWindow),
	_renderTexture(renderTexture),
	_applicationMutex(&mutex),
	_isThreadTerminated(false),
	//_view(),
	_textures(nullptr),
	_fonts(nullptr),
	_keybinding(nullptr),
	_stateStack(nullptr),
	_statisticsText(),
	_statisticsUpdateTime(),
	_statisticsNumFrames(0)
{
	/* TODO uncomment when removing separate windows.*/
	/*_applicationMutex->lock();
	sf::Vector2f mainWindowSize(_mainWindow->getSize());
	_applicationMutex->unlock();
	_view.setSize(mainWindowSize.x / 2, mainWindowSize.y);
	if (uid == 0) {
		_view.setViewport(sf::FloatRect(0, 0, 0.5f, 1.0f));
	}
	else {
		_view.setViewport(sf::FloatRect(0.5f, 0, 0.5f, 1.0f));
	}*/
}

void Client::initialize(int keyBindingConfiguration)
{
	_textures.reset(new TextureHolder());
	_fonts.reset( new FontHolder());
	
	_fonts->load(Fonts::Main, "Media/Sansation.ttf");

	_textures->load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");
	_textures->load(Textures::MenuScreen, "Media/Textures/MenuScreen.png");
	_textures->load(Textures::Buttons, "Media/Textures/Button.png");
	_textures->load(Textures::Car, "Media/Textures/Car.png");
	_textures->load(Textures::Bullet, "Media/Textures/Bullet.png");

	_statisticsText.setFont(_fonts->get(Fonts::Main));
	_statisticsText.setPosition(5.f, 5.f);
	_statisticsText.setCharacterSize(10u);

	_keybinding.reset(new KeyBinding(keyBindingConfiguration));
	_stateStack.reset(new StateStack(State::Context(_uid, *_renderTexture, *_textures, *_fonts, *_keybinding, *_applicationMutex)));
	registerStates();
	_stateStack->pushState(States::Title);
}

void Client::run() {
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timeSinceLastTick = sf::Time::Zero;

	bool isThreadTerminated = false;

	while (!isThreadTerminated)
	{
		_clientMutex.lock();
		isThreadTerminated = _isThreadTerminated;
		_clientMutex.unlock();

		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			/*if (_stateStack->isEmpty())
				_mainWindow->close();*/
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
	_clientMutex.lock();
	_isThreadTerminated = true;
	_clientMutex.unlock();
}

void Client::processInput()
{
	_clientMutex.lock();
	for (size_t i = 0; i < _inputs.size(); i++) {
		_stateStack->handleEvent(_inputs[i]);
	}
	_inputs.clear();
	_clientMutex.unlock();
}

void Client::setInputs(const std::vector <sf::Event>& newInputs) {
	_clientMutex.lock();
	_inputs = newInputs;
	_clientMutex.unlock();
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
	// TODO delete everything except _stateStack->draw(); after replacing _mainWindow by _renderTexture
	_applicationMutex->lock();
	// Seems necessary when there are multiple contexts.
	_mainWindow->setActive(true);
	//_mainWindow->clear();
	_renderTexture->clear();
	_stateStack->draw();
	_renderTexture->display();
	//_mainWindow->setView(_mainWindow->getDefaultView());
	//_mainWindow->draw(_statisticsText);

	//_mainWindow->display();
	_mainWindow->setActive(false);
	_applicationMutex->unlock();
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
}