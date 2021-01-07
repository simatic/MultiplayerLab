#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Common/Systems/System.h"
#include "../Components/Sprite.h"
#include "../Components/Transform.h"
#include "Common/Entity.h"

class GridRenderer : public RenderSystem<Transform>
{
private:
    sf::VertexArray grid;
    sf::Texture& gridElement;
    sf::Transform transform{};
    constexpr static float CROSSES_PER_UNIT = 0.01f;

public:
	explicit GridRenderer(GameManager* const gameManager);

	void update(const sf::Time& dt) override;
};