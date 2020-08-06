#include <KeyBinding.h>
#include <string>
#include <algorithm>


KeyBinding::KeyBinding(int controlPreconfiguration)
	: mKeyMap()
{
	// Set initial key bindings for player 1
	if (controlPreconfiguration == 1)
	{
		mKeyMap[sf::Keyboard::Left] = PlayerAction::MoveLeft;
		mKeyMap[sf::Keyboard::Right] = PlayerAction::MoveRight;
		mKeyMap[sf::Keyboard::Up] = PlayerAction::MoveUp;
		mKeyMap[sf::Keyboard::Down] = PlayerAction::MoveDown;
	}
	else if (controlPreconfiguration == 2)
	{
		// Player 2
		mKeyMap[sf::Keyboard::Q] = PlayerAction::MoveLeft;
		mKeyMap[sf::Keyboard::D] = PlayerAction::MoveRight;
		mKeyMap[sf::Keyboard::Z] = PlayerAction::MoveUp;
		mKeyMap[sf::Keyboard::S] = PlayerAction::MoveDown;
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

sf::Keyboard::Key KeyBinding::getAssignedKey(Action action) const
{
	for(auto pair : mKeyMap)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
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