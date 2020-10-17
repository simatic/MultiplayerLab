#include "State.h"
#include "StateStack.h"


State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
	/*MusicPlayer& music, SoundPlayer& sounds,*/ KeyBinding& keys)
: window(&window)
, textures(&textures)
, fonts(&fonts)
//, music(&music)
//, sounds(&sounds)
, keys(&keys)
{
}

State::State(StateStack& stack, Context context)
: mStack(&stack)
, mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}

bool State::tick()
{
	return true;
}

void State::onActivate()
{

}

void State::onDestroy()
{

}
