#include <Client/States/MenuState.h>
#include <Common/GUI/Button.h>
#include "Utility.h"
//#include <Book/MusicPlayer.hpp>
#include <Common/Resources/ResourceHolder.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
{
	sf::Texture& texture = context.textures->get(Textures::MenuScreen);
	mBackgroundSprite.setTexture(texture);

	auto playButton = std::make_shared<GUI::Button>(context);
	playButton->setPosition(100, 300);
	playButton->setText("Play");
	playButton->setCallback([this] ()
	{
		requestStackPop();
		requestStackPush(States::Game);
	});

	auto hostPlayButton = std::make_shared<GUI::Button>(context);
	hostPlayButton->setPosition(400, 300);
	hostPlayButton->setText("Host");
	hostPlayButton->setCallback([this] ()
	{
		requestStackPop();
		requestStackPush(States::HostGame);
	});

	auto joinPlayButton = std::make_shared<GUI::Button>(context);
	joinPlayButton->setPosition(400, 450);
	joinPlayButton->setText("Join");
	joinPlayButton->setCallback([this] ()
	{
		requestStackPop();
		requestStackPush(States::JoinGame);
	});

	auto settingsButton = std::make_shared<GUI::Button>(context);
	settingsButton->setPosition(100, 450);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this] ()
	{
		requestStackPop();
		requestStackPush(States::Settings);
	});

	auto exitButton = std::make_shared<GUI::Button>(context);
	exitButton->setPosition(100, 600);
	exitButton->setText("Exit");
	exitButton->setCallback([this] ()
	{
		requestStackPop();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(hostPlayButton);
	mGUIContainer.pack(joinPlayButton);
	mGUIContainer.pack(exitButton);

	// Play menu theme
//	context.music->play(Music::MenuTheme);
}

void MenuState::draw()
{
	sf::RenderTarget& target = *getContext().target;

	target.setView(target.getDefaultView());

	target.draw(mBackgroundSprite);
	target.draw(mGUIContainer);
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}
