#ifndef BOOK_APPLICATION_HPP
#define BOOK_APPLICATION_HPP

#include <Common/Resources/ResourceHolder.h>
#include <Common/Resources/ResourceIdentifiers.h>
#include <Common/Resources/KeyBinding.h>
#include <Common/States/StateStack.h>
#include <Client/Client.h>
#include <Common/Utility/ThreadSafeQueue.h>
#include <Profiling.h>
#include "Settings.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <thread>
#include <Server/Server.h>
#include <Server/Interface.h>

class Application
{
public:
		Application();
		void run();

private:
	void render();
	void manageInputs();

	void quitApplication();
		
private:
	// Main window in which the Clients' views will be rendered.
	sf::RenderWindow mWindow;
	bool _isMainWindowOpen;

	States::ID activeState;

	std::unique_ptr<StateStack> stateStack;
	std::unique_ptr<Settings> settings;
};

#endif // BOOK_APPLICATION_HPP
