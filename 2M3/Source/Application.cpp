#include "Application.h"
#include "Utility.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "TitleState.h"
#include "MenuState.h"
#include "GlobalSettingsState.h"
#include "SettingsState.h"
#include "GameState.h"
#include "MainState.h"

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
	_isMainWindowOpen(false),
	stateStack(std::make_unique<StateStack>(State::Context::Context())),
	settings(std::make_unique<Settings>())
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

	stateStack->registerState<MainState, sf::RenderWindow*, Settings*>(States::Main, &mWindow, settings.get());
	stateStack->registerState<GlobalSettingsState, sf::RenderWindow*, Settings*>(States::GlobalSettings, &mWindow, settings.get());
	stateStack->pushState(States::Main);
	activeState = States::Main;
}

void Application::run() {
	while (_isMainWindowOpen) {
	    FrameMark;
		stateStack->update(sf::Time::Zero);
		manageInputs();
		//sf::sleep(sf::seconds(TimePerFrame.asSeconds()));
		render();
	}
}

void Application::manageInputs() {
	sf::Event event;
	while (mWindow.pollEvent(event)) {
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Key::Escape) {
			stateStack->popState();
			if (activeState == States::Main) {
				stateStack->pushState(States::GlobalSettings);
				activeState = States::GlobalSettings;
			}
			else {
				stateStack->pushState(States::Main);
				activeState = States::Main;
			}
		}
		else if (event.type == sf::Event::Closed) {
			quitApplication();
		}
		else {
			stateStack->handleEvent(event);
		}
	}
}

void Application::render() {
	mWindow.clear(sf::Color(15, 15, 15));
	stateStack->draw();
}

void Application::quitApplication() {
    mWindow.close();
    _isMainWindowOpen = false;
}

