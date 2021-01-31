#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Particles.h"

/**
 * @class ParticleRenderer
 * Renders particle at the back of the vehicle.
 */
class ParticleRenderer : public RenderSystem<Transform, Particles>
{
public:
	explicit ParticleRenderer(GameManager* const gameManager);

	void update(const sf::Time& dt) override;;

private:
    void addVertex(sf::VertexArray& shapes, float x, float y, float width, float height, sf::Color& color);
};