#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Particles.h"

class ParticleRenderer : public RenderSystem<Transform, Particles>
{
public:
	void update(const sf::Time& dt) override;;

private:
    void addVertex(sf::VertexArray& shapes, float x, float y, float width, float height, sf::Color& color);
};