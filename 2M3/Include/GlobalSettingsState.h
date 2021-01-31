#pragma once

#include <Common/States/State.h>
#include "Settings.h"

#include <SFML/System/Clock.hpp>

class GlobalSettingsState : public State {
public:
	GlobalSettingsState(StateStack& stack, Context context, sf::RenderWindow* window, Settings* settings);

	bool update(sf::Time dt) override;
	bool handleEvent(const sf::Event& event) override;
	void draw() override;

private:
	bool imguiInit = false;
	bool keyMapInEditMode = false;
	bool playerAKeyEdit[5] = { false, false, false, false, false };
	bool playerBKeyEdit[5] = { false, false, false, false, false };

	sf::RenderWindow* window;
	sf::Clock deltaClock;
	Settings* settings;
};