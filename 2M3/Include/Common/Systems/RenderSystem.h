#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include "Common/Systems/System.h"
#include "../Components/Sprite.h"
#include "../Components/Transform.h"

class RenderSystem : public SignedSystem<Transform, Sprite>
{
public:
	static void render(sf::RenderTarget& target, Transform transform, Sprite sprite);
};