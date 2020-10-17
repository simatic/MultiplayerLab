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

/* On rajoute 2 threads pour chaque client dans ce constructeur. Chaque thread va gérer toute la partie de run,
update, draw, processInput etc en soit, toutes les méthodes sauf le constructeur.
*/
Application::Application()
	: mWindow(sf::VideoMode::getDesktopMode()/*sf::VideoMode(1600, 900)*/, "2M3", sf::Style::Close | sf::Style::Resize)
//, mTextures()
//, mFonts()
//, mMusic()
//, mSounds()
, mKeyBinding1(2)
, mKeyBinding2(1)
//, mStateStack(State::Context(mWindow, mTextures, mFonts, mKeyBinding1, mKeyBinding2))
//, mStatisticsText()
//, mStatisticsUpdateTime()
//, mStatisticsNumFrames(0)
, _clientCount(0)
{
	mWindow.setKeyRepeatEnabled(false);
	mWindow.setVerticalSyncEnabled(true);

	/* Rajouter 1 sf::RenderWindow pour chaque client.
	 On va fusionner thread et Player. Player n'appartient plus à World mais
	 Wolrd sera en-dessous de Player.
	*/
	

	// Tout ce qui suit va passer dans chaque thread.
	/*mFonts.load(Fonts::Main, 				"Media/Sansation.ttf");

	mTextures.load(Textures::TitleScreen,	"Media/Textures/TitleScreen.png");
	mTextures.load(Textures::MenuScreen,	"Media/Textures/MenuScreen.png");
	mTextures.load(Textures::Buttons,		"Media/Textures/Button.png");

	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::Title);*/

}

void Application::run()
{
	//sf::Clock clock;
	//sf::Time timeSinceLastUpdate = sf::Time::Zero;
	//sf::Time timeSinceLastTick = sf::Time::Zero;

	//while (mWindow.isOpen())
	//{
	//	sf::Time dt = clock.restart();
	//	timeSinceLastUpdate += dt;
	//	while (timeSinceLastUpdate > TimePerFrame)
	//	{
	//		timeSinceLastUpdate -= TimePerFrame;

	//		processInput();
	//		update(TimePerFrame);

	//		// Check inside this loop, because stack might be empty before update() call
	//		if (mStateStack.isEmpty())
	//			mWindow.close();
	//	}

	//	timeSinceLastTick += dt;
	//	while (timeSinceLastTick > TimePerTick)
	//	{
	//		timeSinceLastTick -= TimePerTick;

	//		tick();
	//	}

	//	updateStatistics(dt);
	//	render();
	//}
	sf::Thread threadClient1(&Application::launchClientThread, this);
	sf::Thread threadClient2(&Application::launchClientThread, this);
	std::cout << "Launch the thread of Client 1.\n";
	threadClient1.launch();

	std::cout << "Launch the thread of Client 2.\n";
	threadClient2.launch();
	
	while (mWindow.isOpen()) {
		sf::Event event;
		while (mWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
			{
				 // Wait for each threads to terminate.
				threadClient1.wait();
				std::cout << "First thread ended.\n";
				threadClient2.wait();
				std::cout << "Second thread ended.\n";
				//_clients.clear();
				mWindow.close();
			}
		}
	}
}

void Application::launchClientThread() {
	Client* client;
	_mutex.lock();
	int clientUID = _clientCount;
	_clientCount++;
	_mutex.unlock();
	if (clientUID == 0) {
		client = new Client(_clientCount, &mWindow, "Client 1", mKeyBinding1);
	}
	else
	{
		client = new Client(_clientCount, &mWindow, "Client 2", mKeyBinding2);
	}
	client->run();
}

//void Application::processInput()
//{
//	sf::Event event;
//	while (mWindow.pollEvent(event))
//	{
//		mStateStack.handleEvent(event);
//
//		if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
//			mWindow.close();
//	}
//}
//
//void Application::update(sf::Time dt)
//{
//	mStateStack.update(dt);
//}
//
//void Application::tick()
//{
//	mStateStack.tick();
//}
//
//void Application::render()
//{
//	mWindow.clear();
//
//	mStateStack.draw();
//
//	//mWindow.setView(mWindow.getDefaultView());
//	mWindow.draw(mStatisticsText);
//
//	mWindow.display();
//}
//
//void Application::updateStatistics(sf::Time dt)
//{
//	mStatisticsUpdateTime += dt;
//	mStatisticsNumFrames += 1;
//	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
//	{
//		mStatisticsText.setString("FPS: " + toString(mStatisticsNumFrames));
//
//		mStatisticsUpdateTime -= sf::seconds(1.0f);
//		mStatisticsNumFrames = 0;
//	}
//}
//
//void Application::registerStates()
//{
//	mStateStack.registerState<TitleState>(States::Title);
//	mStateStack.registerState<MenuState>(States::Menu);
//	mStateStack.registerState<GameState>(States::Game);
//	mStateStack.registerState<SettingsState>(States::Settings);
//	mStateStack.registerState<MultiplayerGameState>(States::HostGame, true);
//	mStateStack.registerState<MultiplayerGameState>(States::JoinGame, false);
//	//mStateStack.registerState<PauseState>(States::Pause);
//	//mStateStack.registerState<PauseState>(States::NetworkPause, true);
//}
