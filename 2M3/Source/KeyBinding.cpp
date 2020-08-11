#include "KeyBinding.h"
#include "Foreach.h"

#include <string>
#include <algorithm>


KeyBinding::KeyBinding(int controlPreconfiguration)
: mKeyMap()
{
	// Set initial key bindings for player 1
	if (controlPreconfiguration == 1)
	{
		mKeyMap[sf::Keyboard::Left]		= PlayerAction::TurnLeft;
		mKeyMap[sf::Keyboard::Right]	= PlayerAction::TurnRight;
		mKeyMap[sf::Keyboard::Up]		= PlayerAction::Accelerate;
		mKeyMap[sf::Keyboard::Down]		= PlayerAction::Brake;
		mKeyMap[sf::Keyboard::Numpad7]	= PlayerAction::Fire;
		mKeyMap[sf::Keyboard::Numpad8]	= PlayerAction::ChangeWeapon;
	}
	else if (controlPreconfiguration == 2)
	{
		// Player 2
		mKeyMap[sf::Keyboard::Q] = PlayerAction::TurnLeft;
		mKeyMap[sf::Keyboard::D] = PlayerAction::TurnRight;
		mKeyMap[sf::Keyboard::Z] = PlayerAction::Accelerate;
		mKeyMap[sf::Keyboard::S] = PlayerAction::Brake;
		mKeyMap[sf::Keyboard::F] = PlayerAction::Fire;
		mKeyMap[sf::Keyboard::G] = PlayerAction::ChangeWeapon;
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