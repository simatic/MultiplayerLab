#include "State.h"
#include "StateStack.h"


State::Context::Context(int& uid, sf::RenderTexture& target, TextureHolder& textures, FontHolder& fonts,
	/*MusicPlayer& music, SoundPlayer& sounds,*/ KeyBinding& keys, sf::Mutex& appMutex, sf::Color& associatedColor)
: uid(&uid)
, target(&target)
, textures(&textures)
, fonts(&fonts)
//, music(&music)
//, sounds(&sounds)
, keys(&keys)
, mutex(&appMutex)
, associatedColor(associatedColor)
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
