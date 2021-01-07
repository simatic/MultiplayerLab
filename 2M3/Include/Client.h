#pragma once
#include <SFML/Graphics.hpp>

#include <string>

#include "StateStack.h"
#include "KeyBinding.h"

const sf::Time TimePerTick = sf::seconds(1.f / 20.f);
const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

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

	/// Color associated for this client (eg red for player A, green for player B)
	sf::Color getAssociatedColor();

private :
	void processInput();
	void update(sf::Time dt);
	void tick();
	void renderBorder();
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
	sf::Color _associatedColor{};
	sf::RectangleShape _borderRectangle{};
	std::size_t _statisticsNumFrames;
};