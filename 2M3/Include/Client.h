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
	Client(int uid, sf::RenderWindow& mainWindow, sf::Mutex&, sf::RenderTexture*);

	// To call in the thread corresponding to its Client.
	void initialize(int keyBindingConfiguration);

	// Method that will run into the thread corresponding to its Client.
	void run();

	// Method called outside of the Client's Thread (in the main Thread) to terminate properly its Thread.
	void terminate();

	// Method called to pass inputs to this Client.
	void setInputs(const std::vector <sf::Event>& newInputs);

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
	
	//sf::View _view;

	std::unique_ptr<sf::RenderTexture> _renderTexture;
	//sf::RenderTexture* _renderTexture;
	
	// Common mutex - coming from Application - between all the threads to lock Application's variables.
	sf::Mutex* _applicationMutex;

	// Mutex to lock Client's variables.
	sf::Mutex _clientMutex;

	// False if the Thread is still running and true if the Thread has to terminate or is terminated.
	bool _isThreadTerminated;

	// Vector of inputs' type retrieved by Application. The event itself is not needed.
	std::vector <sf::Event> _inputs;

	std::shared_ptr<TextureHolder> _textures;
	std::shared_ptr <FontHolder> _fonts;

	std::shared_ptr <KeyBinding> _keybinding;
	std::unique_ptr <StateStack> _stateStack;

	sf::Text _statisticsText;
	sf::Time _statisticsUpdateTime;
	std::size_t _statisticsNumFrames;
};