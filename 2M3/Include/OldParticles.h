#pragma once
#include <SFML/Graphics.hpp>
#include <deque>

struct OldParticle
{
	sf::Vector2f	position;
	sf::Color		color;
	sf::Time		lifetime;
};

class OldParticles
{
public:
	OldParticles(sf::Color color, sf::Time life);

	void update(sf::Time dt);
	void draw(sf::RenderTarget& target);

	void setPosition(sf::Vector2f pos);

	void addVertex(float x, float y, float width, float height, sf::Color& color);
	void computeArray();

private:
	sf::Vector2f			mPosition;

	std::deque<OldParticle>	mParticles;
	sf::VertexArray			mVertexArray;

	sf::Time				mSpawnDelay;
	sf::Time				mCurrentDelay;

	sf::Color				mColor;
	sf::Time				mLifetime;

};