#include <Collider.h>
#include <Utility.h>

void projectRectangle(sf::Vector2f axis, Rectangle rectangle, float& min, float& max)
{
	min = dotProduct(axis, rectangle.points[0]);
	max = dotProduct(axis, rectangle.points[0]);

	for (int i = 1; i < 4; i++)
	{
		float val = dotProduct(axis, rectangle.points[i]);
		if (val < min) min = val;
		else if (val > max) max = val;
	}
}

float intervalDistance(float minA, float maxA, float minB, float maxB)
{
	if (minA < minB)
	{
		return minB - maxA;
	}
	return minA - maxB;
}

CollisionResult collision(Rectangle rectA, Rectangle rectB, sf::Vector2f velocity)
{
	CollisionResult result;
	result.intersect = true;

	for (int i = 0; i < 2 && result.intersect; i++)
	{
		float minA, minB, maxA, maxB;
		sf::Vector2f axis = unitVector(rotate(rectA.points[i + 1] - rectA.points[i], M_PI_2));

		projectRectangle(axis, rectA, minA, maxA);
		projectRectangle(axis, rectB, minB, maxB);
		float d = intervalDistance(minA, maxA, minB, maxB);

		if (d > 0) result.intersect = false;
	}

	for (int i = 0; i < 2 && result.intersect; i++)
	{
		float minA, minB, maxA, maxB;
		sf::Vector2f axis = unitVector(rotate(rectB.points[i + 1] - rectB.points[i], M_PI_2));

		projectRectangle(axis, rectA, minA, maxA);
		projectRectangle(axis, rectB, minB, maxB);
		float d = intervalDistance(minA, maxA, minB, maxB);

		if (d > 0) result.intersect = false;
	}

	return result;
}