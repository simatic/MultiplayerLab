#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include "../Components/Sprite.h"
#include "../Components/Transform.h"

class RenderSystem
{
public:
	static void render(sf::RenderTarget& target, Transform transform, Sprite sprite);
};