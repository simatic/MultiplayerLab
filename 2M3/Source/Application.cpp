#include "Application.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "StateStack.h"
#include "TitleState.h"
//#include "GameState.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
	: mWindow(sf::VideoMode(640, 480), "States", sf::Style::Close)
	, mTextures()
	, mFonts()
	, mStateStack(State::Context(mWindow, mTextures, mFonts))
{
	mFonts.load(Fonts::Main, "Media/Sansation.ttf");
	mTextures.load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");

	registerStates();
	mStateStack.pushState(States::TitleState);
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::TitleState);
//	mStateStack.registerState<GameState>(States::Game);
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);
		if (event.type == sf::Event::Closed) { mWindow.close(); }
	}
}

void Application::render()
{
	mWindow.clear();
	mStateStack.draw();
	mWindow.setView(mWindow.getDefaultView());
	mWindow.display();
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (mStateStack.isEmpty())
				mWindow.close();
		}
		render();
	}
}
