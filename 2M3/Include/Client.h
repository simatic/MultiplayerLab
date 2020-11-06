#pragma once
#include <SFML/Graphics.hpp>

#include <string>

#include "StateStack.h"
#include "World.h"
#include "KeyBinding.h"

//
class Client
{
public :
	Client(int uid, sf::RenderWindow* mainWindow, std::string, sf::Mutex*);

	// To call in the thread corresponding to its Client.
	void initialize(int keyBindingConfiguration);

	// Method that will run into the thread corresponding to its Client.
	void run();

	// Method called outside of the Client's Thread (in the main Thread) to terminate properly its Thread.
	void terminate();

private :
	void processInput();
	void update(sf::Time dt);
	void tick();
	void render();

	void updateStatistics(sf::Time dt);
	void registerStates();

private :
	// User ID that has to be unique.
	int _uid;

	// Main Window of Application in which _view will be rendered.
	sf::RenderWindow* _mainWindow;
	sf::View _view;
	
	// Common mutex - coming from Application - between all the threads to lock variables.
	sf::Mutex* _applicationMutex;

	// False if the Thread is still running and true if the Thread has to terminate or is terminated.
	bool _isThreadTerminated;

	TextureHolder* _textures;
	FontHolder* _fonts;

	KeyBinding* _keybinding;
	StateStack* _stateStack;

	sf::Text _statisticsText;
	sf::Time _statisticsUpdateTime;
	std::size_t _statisticsNumFrames;
};