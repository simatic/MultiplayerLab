#include "Common/Systems/ParticleSystem.h"

ParticleSystem::ParticleSystem(GameManager* const gameManager) :
	LogicSystem<Transform, CarEngine, Particles>(gameManager)
{}

void ParticleSystem::update(const sf::Time& dt)
{
	for (Entity* entity: entities)
	{
		Transform* transform = entity->getComponent<Transform>();
		Particles* particles = entity->getComponent<Particles>();

		particles->position = transform->position - (float)20 * entity->getComponent<CarEngine>()->direction;

		while (!particles->particlesDeque.empty() && particles->particlesDeque.front().lifetime <= sf::Time::Zero)
		{
			particles->particlesDeque.pop_front();
		}

		for (Particle& particle : particles->particlesDeque)
		{
			particle.lifetime -= dt;
		}

		if (particles->currentDelay > sf::Time::Zero) particles->currentDelay -= dt;
		if (particles->currentDelay <= sf::Time::Zero)
		{
			particles->currentDelay = particles->spawnDelay;
			
			Particle particle;
			particle.position = particles->position;
			particle.color = particles->color;
			particle.lifetime = particles->lifetime;

			particles->particlesDeque.push_back(particle);
		}
	}
}