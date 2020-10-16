#ifndef BOOK_KEYBINDING_HPP
#define BOOK_KEYBINDING_HPP

#include <SFML/Window/Keyboard.hpp>

#include <map>
#include <vector>


namespace PlayerAction
{
	enum Type
	{
		Accelerate,
		Brake,
		TurnRight,
		TurnLeft,
		DoAction,
		ChangeAction,
		Count
	};
}

class KeyBinding
{
	public:
		typedef PlayerAction::Type Action;


	public:
		explicit				KeyBinding();

		void					assignKey(Action action, sf::Keyboard::Key key);
		sf::Keyboard::Key		getAssignedKey(Action action) const;

		bool					checkAction(sf::Keyboard::Key key, Action& out) const;


	private:
		std::map<sf::Keyboard::Key, Action>		mKeyMap;
};

#endif // BOOK_KEYBINDING_HPP
