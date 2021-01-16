#include "Client.h"
#include "TitleState.h"
#include "MenuState.h"
#include "SettingsState.h"
#include "GameState.h"
#include "Utility.h"
#include "Profiling.h"

#include <SFML/OpenGL.hpp>

Client::Client(int uid, sf::Mutex& mutex, int renderTextureWidth, int renderTextureHeight, ThreadSafeQueue<sf::Sprite>& queueToDraw, ThreadSafeQueue<sf::Sprite>& queueToDisplay) :
	_uid(uid),
	_renderTexture(new sf::RenderTexture()),
	_queueToDraw(queueToDraw),
	_queueToDisplay(queueToDisplay),
	_applicationMutex(&mutex),
	_isThreadTerminated(false),
	_textures(nullptr),
	_fonts(nullptr),
	_keybinding(nullptr),
	_stateStack(nullptr),
	_statisticsText(),
	_statisticsUpdateTime(),
	_statisticsNumFrames(0)
{
	_renderTexture->create(renderTextureWidth, renderTextureHeight);
}

void Client::initialize(int keyBindingConfiguration)
{
	_textures.reset(new TextureHolder());
	_fonts.reset( new FontHolder());
	
	_fonts->load(Fonts::Main, "Media/Sansation.ttf");

	_textures->load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");
	_textures->load(Textures::MenuScreen, "Media/Textures/MenuScreen.png");
	_textures->load(Textures::Buttons, "Media/Textures/Button.png");
	_textures->load(Textures::Car, "Media/Textures/Player.png");
	_textures->load(Textures::Bullet, "Media/Textures/Bullet.png");
    _textures->load(Textures::GridElement, "Media/Textures/GridElement.png");

	_statisticsText.setFont(_fonts->get(Fonts::Main));
	_statisticsText.setPosition(5.f, 5.f);
	_statisticsText.setCharacterSize(10u);

    // prepare border
    sf::Color borderColor = sf::Color::Red;
    if(_uid != 0) {
        borderColor = sf::Color::Green;
    }
    _associatedColor = borderColor;

    constexpr float borderWidth = 3.0f;
    const float screenWidth = _renderTexture->getSize().x;
    const float screenHeight = _renderTexture->getSize().y;

    _borderRectangle.setSize(sf::Vector2f(screenWidth-borderWidth*2,screenHeight-borderWidth*2));
    _borderRectangle.setPosition(borderWidth, borderWidth);
    _borderRectangle.setOutlineColor(borderColor);
    _borderRectangle.setOutlineThickness(borderWidth);
    _borderRectangle.setFillColor(sf::Color::Transparent);

    _keybinding.reset(new KeyBinding(keyBindingConfiguration));
	_stateStack.reset(new StateStack(State::Context(_uid, *_renderTexture, *_textures, *_fonts, *_keybinding, *_applicationMutex, _associatedColor)));
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
		    ZoneScopedN("Single-Update");
			timeSinceLastUpdate -= TimePerFrame;
			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			/* Maybe change it with isThreadTerminated = true; instead of _mainWindow->close();
			if (_stateStack->isEmpty())
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
	//_inputs.clear();
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

void Client::renderBorder()
{
    auto savedView = _renderTexture->getView();
    // sets the view to default to remove offsets and scrolling introduced by camera
    // ie. makes the position of the border absolute and no longer relative
    _renderTexture->setView(_renderTexture->getDefaultView());
    _renderTexture->draw(_borderRectangle);
    // sets the view as it was before
    _renderTexture->setView(savedView);
}

void Client::render()
{
	sf::Sprite sprite;
	ZoneScoped;
    {
        ZoneScopedN("Wait before draw");
        _queueToDraw.wait_and_pop(sprite);
    }
    {
        ZoneScopedN("Draw");
        TracyMessageL("Clear");
        _renderTexture->clear();
        TracyMessageL("Draw stack");
        _stateStack->draw();
        TracyMessageL("Draw border");
        renderBorder();
        TracyMessageL("Draw stats");
        _renderTexture->setView(_renderTexture->getDefaultView());
        _renderTexture->draw(_statisticsText);
        TracyMessageL("Display");
        _renderTexture->display();
    }
	sprite.setTexture(_renderTexture->getTexture());
    // force OpenGL flush before giving up sprite
    glFlush();
    _queueToDisplay.push(sprite);
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

sf::Color Client::getAssociatedColor() {
    return _associatedColor;
}
