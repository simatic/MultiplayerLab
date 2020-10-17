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
	Client(int, sf::RenderWindow*, std::string, KeyBinding);
	~Client();

	void run();
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

	sf::RenderWindow* _mainWindow;
	sf::View _view;
	TextureHolder _textures;
	FontHolder _fonts;

	StateStack _stateStack;

	sf::Text _statisticsText;
	sf::Time _statisticsUpdateTime;
	std::size_t _statisticsNumFrames;
};