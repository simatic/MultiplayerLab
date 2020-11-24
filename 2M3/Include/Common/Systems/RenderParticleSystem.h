#pragma once
#include <SFML/System/Time.hpp>
#include "Common/Systems/System.h"
#include "OldEntity.h"
#include "Common/Components/Particles.h"

class RenderParticleSystem : public SignedSystem<Transform, Particles>
{
public:
	static void render(sf::RenderTarget& target, OldEntity* entity);

private:
    static void addVertex(sf::VertexArray& shapes, float x, float y, float width, float height, sf::Color& color);
};