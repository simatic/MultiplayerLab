#pragma once

#include "State.h"
#include "KeyBinding.h"
#include "StateStack.h"
#include <Client/Client.h>
#include "Settings.h"
#include "ThreadSafeQueue.h"
#include "Profiling.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <thread>
#include <Server/Server.h>
#include <Server/Interface.h>

class MainState : public State {
public:
	

	struct ClientInfo
	{
		ClientInfo(int uid, sf::Mutex&, int renderTextureWidth, int renderTextureHeight, sf::Thread* thread);
		std::unique_ptr<Client> client;
		std::unique_ptr<sf::Thread> thread;
		ThreadSafeQueue<sf::Sprite> queueToDraw;
		ThreadSafeQueue<sf::Sprite> queueToDisplay;
	};

public:
	MainState(StateStack& stack, Context context, sf::RenderWindow* window, Settings* settings);
	~MainState();

	bool update(sf::Time dt) override;
	bool handleEvent(const sf::Event& event) override;
	void draw() override;

private:
	void launchClientThread();
	void addClientThread();
	void terminateClientThreads();

	//void manageInputs();

	void launchServer();
	void terminateServer();

	//void quitApplication();

private:
	/// How much of vertical space the game should take?
	/// The graphs will take 1-this value to know how much they take vertically
	constexpr static float gameHeightRatio = 0.6f;

	sf::RenderWindow* window;

	// Replace mWindow.isOpen() to avoid blocking the access to mWindow from other threads.
	bool _isMainWindowOpen;

	std::vector<std::unique_ptr<ClientInfo>> _clientsInfo;

	// Number of Clients in _clients.
	int _clientCount;

	// Common mutex between all the threads to lock variables.
	std::unique_ptr<sf::Mutex> _mutex;

	/* Index of the last launched Thread to keep a track of launched Threads
	 * and not yet launched Threads.
	*/
	int _clientThreadLaunchedIndex;

	// Vector of inputs' type to give to the Clients. The event itself is not needed.
	std::vector <sf::Event> _clientInputs;

	/// Thread executing the game server
	std::thread serverThread;

	/// Weak server reference to stop the server when the application is closed
	std::weak_ptr<Server> serverReference{};

	std::unique_ptr<Interface> serverInterface = nullptr;

	/// Settings struct to hold keybindings
	Settings* settings;
};
