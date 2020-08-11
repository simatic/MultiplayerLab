#pragma once
#include <SFML/Graphics.hpp>

struct CollisionResult
{
	bool willIntersect;
	bool intersect;
	sf::Vector2f speedReductionA;
	sf::Vector2f speedReductionB;
};

struct Rectangle
{
	std::vector<sf::Vector2f> points;
};

void projectRectangle(sf::Vector2f axis, Rectangle rectangle, float& min, float& max);
float intervalDistance(float minA, float maxA, float minB, float maxB);

CollisionResult collision(Rectangle rectA, Rectangle rectB, sf::Vector2f velocityA, sf::Vector2f velocityB, sf::Time dt);