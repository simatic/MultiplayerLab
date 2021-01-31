#ifndef BOOK_SETTINGSSTATE_HPP
#define BOOK_SETTINGSSTATE_HPP

#include <Common/States/State.h>
#include <Common/Resources/KeyBinding.h>
#include <Common/GUI/Container.h>
#include <Common/GUI/Button.h>
#include <Common/GUI/Label.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>


class SettingsState : public State
{
	public:
										SettingsState(StateStack& stack, Context context);

		virtual void					draw();
		virtual bool					update(sf::Time dt);
		virtual bool					handleEvent(const sf::Event& event);


	private:
		void							updateLabels();
		void							addButtonLabel(std::size_t index, std::size_t x, std::size_t y, const std::string& text, Context context);


	private:
		sf::Sprite											mBackgroundSprite;
		GUI::Container										mGUIContainer;
		std::array<GUI::Button::Ptr, 2 * PlayerAction::Count>	mBindingButtons;
		std::array<GUI::Label::Ptr, 2 * PlayerAction::Count> 	mBindingLabels;
};

#endif // BOOK_SETTINGSSTATE_HPP
