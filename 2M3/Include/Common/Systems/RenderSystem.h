#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include "Common/Systems/System.h"
#include "../Components/Sprite.h"
#include "../Components/Transform.h"
#include "OldEntity.h"

class RenderSystem : public SignedSystem<Transform, Sprite>
{
public:
	static void render(OldEntity* entity, sf::RenderTarget& target, Transform transform);
};