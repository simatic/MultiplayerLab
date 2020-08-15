#include <PlayerGUI.h>
#include <iostream>

namespace GUI
{
	PlayerGUI::PlayerGUI(const FontHolder& fonts) :
		mAction("Shoot", fonts)
	{

	}

	PlayerGUI::PlayerGUI(Player* player, const FontHolder& fonts) :
		mPlayer(player),
		mAction("Shoot", fonts),
		mSpeedometer(),
		mSpeedometerBackground()
	{
		if (mPlayer->getID() == 0) setPosition(0, 800);
		else setPosition(800, 800);

		mAction.setPosition(getPosition());

		mSpeedometerBackground.setRadius(100);
		mSpeedometerBackground.setOrigin(100, 100);
		mSpeedometerBackground.setFillColor(sf::Color::White);
		mSpeedometerBackground.setPosition(getPosition() + sf::Vector2f(800, 100));

		mSpeedometer.setSize(sf::Vector2f(90, 5));
		mSpeedometer.setOrigin(0, 2.5f);
		mSpeedometer.setPosition(mSpeedometerBackground.getPosition());
		mSpeedometer.setRotation(180.f);
		mSpeedometer.setFillColor(sf::Color::Black);
	}

	void GUI::PlayerGUI::initialize(Player* player)
	{
		mPlayer = player;
		if (mPlayer->getID() == 0) setPosition(0, 800);
		else setPosition(800, 800);

		mAction.setPosition(getPosition());

		mSpeedometerBackground.setRadius(100);
		mSpeedometerBackground.setOrigin(100, 100);
		mSpeedometerBackground.setFillColor(sf::Color::White);
		mSpeedometerBackground.setPosition(getPosition() + sf::Vector2f(800, 100));

		mSpeedometer.setSize(sf::Vector2f(90, 5));
		mSpeedometer.setOrigin(0, 2.5f);
		mSpeedometer.setPosition(mSpeedometerBackground.getPosition());
		mSpeedometer.setRotation(180.f);
		mSpeedometer.setFillColor(sf::Color::Black);
	}

	bool PlayerGUI::isSelectable() const
	{
		return false;
	}

	void PlayerGUI::handleEvent(const sf::Event&)
	{
	}

	void PlayerGUI::updateElements(sf::RenderTarget& target)
	{
		Car* car = mPlayer->getCar();

		if (mPlayer->getID() == 0) setPosition(target.mapPixelToCoords(sf::Vector2i(0, 800)));
		else setPosition(target.mapPixelToCoords(sf::Vector2i(800, 800)));

		mAction.setPosition(getPosition());
		mSpeedometerBackground.setPosition(getPosition() + sf::Vector2f(400, 100));
		mSpeedometer.setPosition(mSpeedometerBackground.getPosition());

		mAction.setText(car->getActionText());
		mSpeedometer.setRotation(180.f + 180.f * car->getSpeedRatio());
	}

	void PlayerGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//states.transform *= getTransform();

		target.draw(mAction, states);
		target.draw(mSpeedometerBackground, states);
		target.draw(mSpeedometer, states);
	}

}