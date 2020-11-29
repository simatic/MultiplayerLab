#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include "Common/Systems/System.h"
#include "../Components/Sprite.h"
#include "../Components/Transform.h"
#include "Common/Entity.h"

class RenderSystem : public SignedSystem<Transform, Sprite>
{
public:
	void update(const sf::Time& dt) override;
};