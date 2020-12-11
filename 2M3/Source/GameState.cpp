#include "GameState.h"

#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, mWorld(*context.uid ,*context.target, context.keys, *context.fonts, true)
	, mClock()
{	
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	return mWorld.handleEvent(event);
}

