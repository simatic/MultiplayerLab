#include "KeyBinding.h"

#include <string>
#include <algorithm>


KeyBinding::KeyBinding(int controlPreconfiguration)
: mKeyMap()
{
	if (controlPreconfiguration == 1)
	{
		mKeyMap[sf::Keyboard::Left] = PlayerAction::Type::TurnLeft;
		mKeyMap[sf::Keyboard::Right] = PlayerAction::Type::TurnRight;
		mKeyMap[sf::Keyboard::Up] = PlayerAction::Type::Accelerate;
		mKeyMap[sf::Keyboard::Down] = PlayerAction::Type::Brake;
		mKeyMap[sf::Keyboard::RControl] = PlayerAction::Type::DoAction;
		mKeyMap[sf::Keyboard::RShift] = PlayerAction::Type::ChangeAction;
	}
	else if (controlPreconfiguration == 2)
	{
		mKeyMap[sf::Keyboard::Q] = PlayerAction::Type::TurnLeft;
		mKeyMap[sf::Keyboard::D] = PlayerAction::Type::TurnRight;
		mKeyMap[sf::Keyboard::Z] = PlayerAction::Type::Accelerate;
		mKeyMap[sf::Keyboard::S] = PlayerAction::Type::Brake;
		mKeyMap[sf::Keyboard::F] = PlayerAction::Type::DoAction;
		mKeyMap[sf::Keyboard::G] = PlayerAction::Type::ChangeAction;
	}
}

void KeyBinding::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyMap.begin(); itr != mKeyMap.end(); )
	{
		if (itr->second == action)
			mKeyMap.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyMap[key] = action;
}


bool KeyBinding::checkAction(sf::Keyboard::Key key, Action& out) const
{
	auto found = mKeyMap.find(key);
	if (found == mKeyMap.end())
	{
		return false;
	}
	else
	{
		out = found->second;
		return true;
	}
}

sf::Keyboard::Key KeyBinding::getAssignedKey(Action action) const
{
	for (auto pair : mKeyMap)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}