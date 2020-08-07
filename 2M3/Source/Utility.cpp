#include <Utility.h>

float length(sf::Vector2f vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float dotProduct(sf::Vector2f u, sf::Vector2f v)
{
	return u.x * v.x + u.y * v.y;
}

sf::Vector2f rotate(sf::Vector2f v, float a)
{
	if (v.x == 0 && v.y == 0) return sf::Vector2f(0, 0);
	float l = length(v);
	if (v.x == 0)
	{
		float prevA = M_PI_2 * v.y / abs(v.y);
		return l * sf::Vector2f(cos(prevA + a), -sin(prevA + a));
	}

	float prevA = -atan2(v.y, v.x);
	return l * sf::Vector2f(cos(prevA + a), -sin(prevA + a));
}

float toRadians(float a)
{
	return M_PI * a / 180.0;
}

float toDegrees(float a)
{
	return 180.0 * a / M_PI;
}

sf::Vector2f unitVector(sf::Vector2f v)
{
	float l = length(v);
	if (l == 0) return v;
	return v / l;
}