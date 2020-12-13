#include "Common/Components/GUI.h"

GUI::GUI(Entity* car) :
    car(car)
{
    mSpeedometerBackground.setRadius(100);
	mSpeedometerBackground.setOrigin(100, 100);
	mSpeedometerBackground.setFillColor(sf::Color::White);
	mSpeedometerBackground.setPosition(sf::Vector2f(800, 100));

	mSpeedometer.setSize(sf::Vector2f(90, 5));
	mSpeedometer.setOrigin(0, 2.5f);
	mSpeedometer.setPosition(mSpeedometerBackground.getPosition());
	mSpeedometer.setRotation(180.f);
	mSpeedometer.setFillColor(sf::Color::Black);
}