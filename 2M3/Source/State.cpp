#include "State.h"
#include "StateStack.h"

State::State(StateStack& stack, Context context)
	: mStack(&stack)
	, mContext(context)
{
}

State::~State()
{
}

State::Context::Context(sf::RenderWindow& window, TextureHolder& texture, FontHolder& fonts)//, Player& player)
	: window(&window)
	, textures(&texture)
	, fonts(&fonts)
//	, player(&player)
{
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}