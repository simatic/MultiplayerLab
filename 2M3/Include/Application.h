#pragma once
#include "StateStack.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"


#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Application 
{
public:
	Application();
	void registerStates();
	void processInput();
	void update(sf::Time dt);	
	void render();
	void run();
private:
	StateStack mStateStack;

	sf::RenderWindow mWindow;
	TextureHolder mTextures;	
	FontHolder mFonts;

	static const sf::Time TimePerFrame;
};