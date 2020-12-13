#include <OldParticles.h>

OldParticles::OldParticles(sf::Color color, sf::Time life) :
	mColor(color),
	mLifetime(life),
	mVertexArray(sf::Quads),
	mSpawnDelay(sf::seconds(1.0 / 40.0))
{
}

void OldParticles::update(sf::Time dt)
{
	while (!mParticles.empty() && mParticles.front().lifetime <= sf::Time::Zero)
	{
		mParticles.pop_front();
	}

	for (OldParticle& particle : mParticles)
	{
		particle.lifetime -= dt;
	}

	if (mCurrentDelay > sf::Time::Zero) mCurrentDelay -= dt;
	if (mCurrentDelay <= sf::Time::Zero)
	{
		mCurrentDelay = mSpawnDelay;
		
		OldParticle particle;
		particle.position = mPosition;
		particle.color = mColor;
		particle.lifetime = mLifetime;

		mParticles.push_back(particle);
	}

}

void OldParticles::draw(sf::RenderTarget& target)
{
	computeArray();
	target.draw(mVertexArray);
}

void OldParticles::setPosition(sf::Vector2f pos)
{
	mPosition = pos;
}

void OldParticles::addVertex(float x, float y, float width, float height, sf::Color& color)
{
	sf::Vertex vertex;
	vertex.position = sf::Vector2f(x, y);
	vertex.texCoords = sf::Vector2f(width, height);
	vertex.color = color;
	mVertexArray.append(vertex);
}

void OldParticles::computeArray()
{
	sf::Vector2f size(10, 10);
	sf::Vector2f half = size / 2.f;

	mVertexArray.clear();
	for(const OldParticle& particle : mParticles)
	{
		sf::Vector2f pos = particle.position;
		sf::Color color = particle.color;

		float ratio = particle.lifetime.asSeconds() / mLifetime.asSeconds();
		color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));

		addVertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, color);
		addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, color);
		addVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
		addVertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, color);
	}
}