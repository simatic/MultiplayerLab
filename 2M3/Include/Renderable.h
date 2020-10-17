#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceIdentifiers.h"

class Renderable
{
public:
	virtual void draw(sf::RenderTarget& target) = 0;
};