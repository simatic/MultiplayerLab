#include "Common/Systems/ParticleRenderer.h"
#include "Common/Managers/GameManager.h"

ParticleRenderer::ParticleRenderer(GameManager* const gameManager) :
	RenderSystem<Transform, Particles>(gameManager)
{}

void ParticleRenderer::update(const sf::Time&)
{
	for (Entity* entity: entities)
	{
		Particles* particles = entity->getComponent<Particles>();

		sf::Vector2f size(10, 10);
		sf::Vector2f half = size / 2.f;

		particles->shapes.clear();
		for(const Particle& particle : particles->particlesDeque)
		{
			sf::Vector2f pos = particle.position;
			sf::Color color = particle.color;

			float ratio = particle.lifetime.asSeconds() / particles->lifetime.asSeconds();
			color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));

			addVertex(particles->shapes, pos.x - half.x, pos.y - half.y, 0.f, 0.f, color);
			addVertex(particles->shapes, pos.x + half.x, pos.y - half.y, size.x, 0.f, color);
			addVertex(particles->shapes, pos.x + half.x, pos.y + half.y, size.x, size.y, color);
			addVertex(particles->shapes, pos.x - half.x, pos.y + half.y, 0.f, size.y, color);
		}

		gameManager->getRenderTarget()->draw(particles->shapes);
	}
}

void ParticleRenderer::addVertex(sf::VertexArray& shapes, float x, float y, float width, float height, sf::Color& color)
{
    sf::Vertex vertex;
	vertex.position = sf::Vector2f(x, y);
	vertex.texCoords = sf::Vector2f(width, height);
	vertex.color = color;
	shapes.append(vertex);
}