#include "StateStack.h"

StateStack::StateStack(State::Context context)
:mStack()
, mPendingList()
, mContext(context)
, mFactories()
{
}

void StateStack::update(sf::Time dt)
{
	for (auto state = mStack.begin(); state != mStack.end(); state++)
	{
		if (!(*state)->update(dt)) { break; }
	}
	applyPendingChanges();
}

void StateStack::draw()
{
	for (State::Ptr& state : mStack)
	{
		state->draw();
	}
}

void StateStack::handleEvent(const sf::Event& event)
{
	for (auto state = mStack.begin(); state != mStack.end(); state++) 
	{
		if (!(*state)->handleEvent(event)) { break; }
	}
	applyPendingChanges();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
	: action(action)
	, stateID(stateID)
{
}

void StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (auto change : mPendingList)
	{
		switch (change.action)
		{
		case Push:
			mStack.push_back(createState(change.stateID));
			break;
		case Pop:
			mStack.pop_back();
			break;
		case Clear:
			mStack.clear();
			break;	
		}
	}
	mPendingList.clear();
}