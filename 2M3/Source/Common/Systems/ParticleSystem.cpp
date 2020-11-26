#include "Common/Systems/ParticleSystem.h"

void ParticleSystem::update(Car* entity, const sf::Time& dt)
{
	Transform* transform = entity->getComponent<Transform>();
    Particles* particles = entity->getComponent<Particles>();

	particles->position = transform->position - (float)20 * entity->getCarDirection();

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