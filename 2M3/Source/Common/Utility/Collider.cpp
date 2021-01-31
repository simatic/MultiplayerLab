#include <Common/Utility/Collider.h>
#include <Common/Utility/Utility.h>

void projectRectangle(sf::Vector2f axis, const Rectangle& rectangle, float& min, float& max)
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

CollisionResult collision(const Rectangle& rectA, const Rectangle& rectB, sf::Vector2f velocityA, sf::Vector2f velocityB, sf::Time dt)
{
	CollisionResult result = { true, true, sf::Vector2f(0, 0), sf::Vector2f(0, 0) };

	float minInterval = INFINITY;
	sf::Vector2f speedReductionAxis = sf::Vector2f(0, 0);

	for (size_t i = 0; i < 2 && result.intersect && result.willIntersect; i++)
	{
		//check current collision
		float minA, minB, maxA, maxB;
		sf::Vector2f axis = unitVector(rotate(rectA.points[i + 1] - rectA.points[i], M_PI_2));

		projectRectangle(axis, rectA, minA, maxA);
		projectRectangle(axis, rectB, minB, maxB);
		float d = intervalDistance(minA, maxA, minB, maxB);

		if (d > 0) result.intersect = false;

		//check future collision
		float projectedVelA = dotProduct(velocityA * dt.asSeconds(), axis);
		float projectedVelB = dotProduct(velocityB * dt.asSeconds(), axis);

		if (projectedVelA < 0) minA += projectedVelA;
		else maxA += projectedVelA;

		if (projectedVelB < 0) minB += projectedVelB;
		else maxB += projectedVelB;

		d = intervalDistance(minA, maxA, minB, maxB);

		if (d > 0)
		{
			result.willIntersect = false;
			break;
		}

		//compute minimal speed reduction
		if (d < minInterval)
		{
			minInterval = d;
			speedReductionAxis = axis;
		}
	}

	//same for rectangle B axes
	for (size_t i = 0; i < 2 && result.intersect && result.willIntersect; i++)
	{
		float minA, minB, maxA, maxB;
		sf::Vector2f axis = unitVector(rotate(rectB.points[i + 1] - rectB.points[i], M_PI_2));

		projectRectangle(axis, rectA, minA, maxA);
		projectRectangle(axis, rectB, minB, maxB);
		float d = intervalDistance(minA, maxA, minB, maxB);

		if (d > 0) result.intersect = false;

		float projectedVelA = dotProduct(velocityA * dt.asSeconds(), axis);
		float projectedVelB = dotProduct(velocityB * dt.asSeconds(), axis);

		if (projectedVelA < 0) minA += projectedVelA;
		else maxA += projectedVelA;

		if (projectedVelB < 0) minB += projectedVelB;
		else maxB += projectedVelB;

		d = intervalDistance(minA, maxA, minB, maxB);

		if (d > 0)
		{
			result.willIntersect = false;
			break;
		}

		if (d < minInterval)
		{
			minInterval = d;
			speedReductionAxis = axis;
		}
	}

	if (result.willIntersect)
	{
		sf::Vector2f unitA = unitVector(velocityA);
		sf::Vector2f unitB = unitVector(velocityB);

		float speedA = dotProduct(unitA, minInterval * speedReductionAxis);
		float speedB = dotProduct(unitB, minInterval * speedReductionAxis);

		result.speedReductionA = unitA * speedA;
		result.speedReductionB = unitB * speedB;
	}

	return result;
}